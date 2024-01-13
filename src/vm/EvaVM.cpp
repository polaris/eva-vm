#include "EvaVM.h"

EvaValue EvaVM::exec([[maybe_unused]] const std::string& program) {
  // auto ast = parser->parse(program);
  // code = compiler->compile(ast);

  constants.push_back(Number(42));

  code = {to_uint8(OpCode::OP_CONST), 0, to_uint8(OpCode::OP_HALT)};

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
      default:
        throw UnknownOpcode(opcode);
    }
  }
}
