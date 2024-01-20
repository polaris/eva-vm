#include "EvaVM.h"

#include <functional>
#include <unordered_map>

#include "UnknowOpcode.h"

EvaVM::EvaVM()
    : parser(std::make_unique<syntax::EvaParser>()),
      compiler(std::make_unique<EvaCompiler>()) {}

EvaValue EvaVM::exec([[maybe_unused]] const std::string& program) {
  const auto ast = parser->parse(program);

  co = compiler->compile(ast);

  // constants.push_back(AllocString("Hello, "));
  // constants.push_back(AllocString("World!"));

  // code = {to_uint8(OpCode::OP_CONST),  0,
  //         to_uint8(OpCode::OP_CONST),  1,
  //         to_uint8(OpCode::OP_CONCAT), to_uint8(OpCode::OP_HALT)};

  // constants.push_back(Number(120));
  // constants.push_back(Number(100));
  // constants.push_back(Number(10));
  // constants.push_back(Number(2));
  // constants.push_back(Number(10));

  // code = {to_uint8(OpCode::OP_CONST),
  //         0,
  //         to_uint8(OpCode::OP_CONST),
  //         1,
  //         to_uint8(OpCode::OP_ADD),
  //         to_uint8(OpCode::OP_CONST),
  //         2,
  //         to_uint8(OpCode::OP_SUB),
  //         to_uint8(OpCode::OP_CONST),
  //         3,
  //         to_uint8(OpCode::OP_MUL),
  //         to_uint8(OpCode::OP_CONST),
  //         4,
  //         to_uint8(OpCode::OP_DIV),
  //         to_uint8(OpCode::OP_HALT)};

  ip = &co->code[0];

  sp = &stack[0];

  return eval();
}

EvaValue EvaVM::eval() {
  static const std::unordered_map<OpCode, std::function<void()>> operations = {
      {OpCode::Const, [this] { push(getConst()); }},
      {OpCode::Concat, [this] { concat(); }},
      {OpCode::Add, [this] { binaryOp(std::plus<>()); }},
      {OpCode::Sub, [this] { binaryOp(std::minus<>()); }},
      {OpCode::Mul, [this] { binaryOp(std::multiplies<>()); }},
      {OpCode::Div, [this] { binaryOp(std::divides<>()); }},
      {OpCode::Compare, [this] { compareOp(); }},
  };

  for (;;) {
    const auto opcode = to_opcode(readByte());
    switch (opcode) {
      case OpCode::Halt:
        return pop();
      default:
        auto op = operations.find(opcode);
        if (op != operations.end()) {
          op->second();
        } else {
          throw UnknownOpcode(opcode);
        }
        break;
    }
  }
}
