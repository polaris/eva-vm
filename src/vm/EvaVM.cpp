#include "EvaVM.h"

EvaValue EvaVM::exec([[maybe_unused]] const std::string& program) {
  // auto ast = parser->parse(program);
  // code = compiler->compile(ast);

  constants.push_back(Number(40));
  constants.push_back(Number(2));

  code = {to_uint8(OpCode::OP_CONST), 0,
          to_uint8(OpCode::OP_CONST), 1,
          to_uint8(OpCode::OP_ADD),   to_uint8(OpCode::OP_HALT)};

  ip = &code[0];

  sp = &stack[0];

  return eval();
}

EvaValue EvaVM::eval() {
  for (;;) {
    const auto opcode = to_opcode(readByte());
    switch (opcode) {
      case OpCode::OP_HALT:
        return pop();
      case OpCode::OP_CONST:
        push(getConst());
        break;
      case OpCode::OP_ADD:
        push(add());
        break;
      default:
        throw UnknownOpcode(opcode);
    }
  }
}

EvaValue EvaVM::add() {
  const auto op1 = asNumber(pop());
  const auto op2 = asNumber(pop());
  const auto result = op1 + op2;
  return Number(result);
}
