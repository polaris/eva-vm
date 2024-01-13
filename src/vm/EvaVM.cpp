#include "EvaVM.h"

#include <functional>
#include <unordered_map>

EvaValue EvaVM::exec([[maybe_unused]] const std::string& program) {
  // auto ast = parser->parse(program);
  // code = compiler->compile(ast);

  constants.push_back(AllocString("Hello, "));
  constants.push_back(AllocString("World!"));

  code = {to_uint8(OpCode::OP_CONST),  0,
          to_uint8(OpCode::OP_CONST),  1,
          to_uint8(OpCode::OP_CONCAT), to_uint8(OpCode::OP_HALT)};

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

  ip = &code[0];

  sp = &stack[0];

  return eval();
}

EvaValue EvaVM::eval() {
  static const std::unordered_map<OpCode, std::function<void()>> operations = {
      {OpCode::OP_CONST, [this] { push(getConst()); }},
      {OpCode::OP_CONCAT, [this] { concat(); }},
      {OpCode::OP_ADD, [this] { binaryOp(std::plus<>()); }},
      {OpCode::OP_SUB, [this] { binaryOp(std::minus<>()); }},
      {OpCode::OP_MUL, [this] { binaryOp(std::multiplies<>()); }},
      {OpCode::OP_DIV, [this] { binaryOp(std::divides<>()); }},
  };

  for (;;) {
    const auto opcode = to_opcode(readByte());
    switch (opcode) {
      case OpCode::OP_HALT:
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
