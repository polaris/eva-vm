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

  EvaValue exec(const std::string& program);

  EvaValue eval();

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