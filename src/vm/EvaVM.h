#ifndef EVAVM_H
#define EVAVM_H

#include <string>
#include <vector>

#include "OpCode.h"
#include "UnknowOpcode.h"

// Eva Virtual Machine
class EvaVM {
 public:
  EvaVM() {}

  void exec([[maybe_unused]] const std::string& program) {
    // auto ast = parser->parse(program);
    // code = compiler->compile(ast);

    code = {OpCode::OP_FOO};

    ip = &code[0];

    return eval();
  }

  void eval() {
    for (;;) {
      const auto opcode = readByte();
      switch (opcode) {
        case OpCode::OP_HALT:
          return;
        default:
          throw UnknownOpcode(opcode);
      }
    }
  }

 private:
  inline OpCode readByte() { return *ip++; }

  OpCode* ip;

  std::vector<OpCode> code;
};

#endif  // EVAVM_H