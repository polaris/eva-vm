#include "EvaVM.h"

#include <functional>
#include <unordered_map>

#include "UnknowOpcode.h"

EvaVM::EvaVM()
    : parser(std::make_unique<syntax::EvaParser>()),
      global(std::make_shared<Global>()),
      compiler(std::make_unique<EvaCompiler>(global)) {
  setGlobalVariables();
}

EvaValue EvaVM::exec([[maybe_unused]] const std::string &program) {
  const auto ast = parser->parse("(begin " + program + ")");
  co = compiler->compile(ast);
  ip = &co->code[0];
  sp = &stack[0];
  bp = sp;
  compiler->disassembleByteCode();
  return eval();
}

void EvaVM::setGlobalVariables() {
  global->addConst("x", 12.3);
}

EvaValue EvaVM::eval() {
  for (;;) {
    const auto opcode = to_opcode(readByte());
    switch (opcode) {
      case OpCode::Halt:
        return pop();
      case OpCode::JmpIfFalse:
        handleJmpIfFalse();
        break;
      case OpCode::Jmp:
        handleJmp();
        break;
      case OpCode::GetGlobal:
        handleGetGlobal();
        break;
      case OpCode::SetGlobal:
        handleSetGlobal();
        break;
      case OpCode::Pop:
        handlePop();
        break;
      case OpCode::GetLocal:
        handleGetLocal();
        break;
      case OpCode::SetLocal:
        handleSetLocal();
        break;
      case OpCode::ScopeExit:
        handleScopeExit();
        break;
      default:
        handleDefault(opcode);
        break;
    }
  }
}

void EvaVM::handleJmpIfFalse() {
  const auto cond = asBoolean(pop());
  const auto address = readShort();
  if (!cond) {
    ip = toAddress(address);
  }
}

void EvaVM::handleJmp() { ip = toAddress(readShort()); }

void EvaVM::handleGetGlobal() {
  const auto globalIndex = readByte();
  push(global->get(globalIndex).value);
}

void EvaVM::handleSetGlobal() {
  const auto globalIndex = readByte();
  const auto value = peek(0);
  global->set(globalIndex, value);
}

void EvaVM::handleGetLocal() {
  const auto localIndex = readByte();
  push(bp[localIndex]);
}

void EvaVM::handleSetLocal() {
  const auto localIndex = readByte();
  const auto value = peek(0);
  bp[localIndex] = value;
}

void EvaVM::handleScopeExit() {
  const size_t varCount = readByte();
  *(sp - (varCount - 1)) = peek(0);
  popN(varCount - 1);
}

void EvaVM::handlePop() {
  pop();
}

void EvaVM::handleDefault(const OpCode& opcode) {
  static const std::unordered_map<OpCode, std::function<void()>> operations = {
      {OpCode::Const, [this] { push(getConst()); }},
      {OpCode::Concat, [this] { concat(); }},
      {OpCode::Add, [this] { binaryOp(std::plus<>()); }},
      {OpCode::Sub, [this] { binaryOp(std::minus<>()); }},
      {OpCode::Mul, [this] { binaryOp(std::multiplies<>()); }},
      {OpCode::Div, [this] { binaryOp(std::divides<>()); }},
      {OpCode::Compare, [this] { compareOp(); }},
  };
  auto op = operations.find(opcode);
  if (op != operations.end()) {
    op->second();
  } else {
    throw UnknownOpcode(opcode);
  }
}
