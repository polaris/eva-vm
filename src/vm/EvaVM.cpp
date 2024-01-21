#include "EvaVM.h"

#include <functional>
#include <unordered_map>

#include "UnknowOpcode.h"

EvaVM::EvaVM()
    : parser(std::make_unique<syntax::EvaParser>()),
      compiler(std::make_unique<EvaCompiler>()) {}

EvaValue EvaVM::exec([[maybe_unused]] const std::string &program) {
  const auto ast = parser->parse(program);
  co = compiler->compile(ast);
  ip = &co->code[0];
  sp = &stack[0];
  return eval();
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

void EvaVM::handleDefault(const OpCode &opcode) {
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
