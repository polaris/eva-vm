#ifndef EVAVM_H
#define EVAVM_H

#include <array>
#include <string>
#include <vector>

#include "EmptyStack.h"
#include "EvaValue.h"
#include "OpCode.h"
#include "StackOverflow.h"
#include "UnknowOpcode.h"

#define STACK_LIMIT 512

// Eva Virtual Machine
class EvaVM {
 public:
  EvaVM() {}

  EvaValue exec([[maybe_unused]] const std::string& program) {
    // auto ast = parser->parse(program);
    // code = compiler->compile(ast);

    constants.push_back(Number(42));

    code = {to_uint8(OpCode::OP_CONST), 0, to_uint8(OpCode::OP_HALT)};

    ip = &code[0];

    sp = &stack[0];

    return eval();
  }

  EvaValue eval() {
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

 private:
  inline uint8_t readByte() { return *ip++; }

  inline EvaValue getConst() {
    auto constIndex = readByte();
    return constants[constIndex];
  }

  inline void push(const EvaValue& value) {
    if ((sp - stack.begin()) == STACK_LIMIT) {
      throw StackOverflow();
    }
    *sp = value;
    sp++;
  }

  inline EvaValue pop() {
    if (sp == stack.begin()) {
      throw EmptyStack();
    }
    --sp;
    return *sp;
  }

  uint8_t* ip;

  EvaValue* sp;

  std::vector<uint8_t> code;

  std::array<EvaValue, STACK_LIMIT> stack;

  std::vector<EvaValue> constants;
};

#endif  // EVAVM_H