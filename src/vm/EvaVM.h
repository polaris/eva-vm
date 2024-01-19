#ifndef EVAVM_H
#define EVAVM_H

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "EmptyStack.h"
#include "EvaCompiler.h"
#include "EvaParser.h"
#include "EvaValue.h"
#include "OpCode.h"
#include "StackOverflow.h"

#define STACK_LIMIT 512

// Eva Virtual Machine
class EvaVM {
 public:
  EvaVM();

  EvaValue exec(const std::string& program);

 private:
  EvaValue eval();

  template <typename Operation>
  void binaryOp(Operation op) {
    const auto op2 = asNumber(pop());
    const auto op1 = asNumber(pop());
    push(Number(op(op1, op2)));
  }

  void compareOp() {
    const auto op = readByte();
    const auto op2 = pop();
    const auto op1 = pop();
    if (isNumber(op1) && isNumber(op2)) {
      const auto v1 = asNumber(op1);
      const auto v2 = asNumber(op2);
      compareValues(op, v1, v2);
    } else if (isString(op1) && isString(op2)) {
      const auto s1 = asCppString(op1);
      const auto s2 = asCppString(op2);
      compareValues(op, s1, s2);
    }
  }

  template <typename T>
  void compareValues(uint8_t op, T v1, T v2) {
    bool result;
    switch (op) {
      case 0:
        result = v1 < v2;
        break;
      case 1:
        result = v1 > v2;
        break;
      case 2:
        result = v1 == v2;
        break;
      case 3:
        result = v1 >= v2;
        break;
      case 4:
        result = v1 <= v2;
        break;
      case 5:
        result = v1 != v2;
        break;
    }
    push(Boolean(result));
  }

  void concat() {
    const auto op2 = asCppString(pop());
    const auto op1 = asCppString(pop());
    push(allocString(op1 + op2));
  }

  inline uint8_t readByte() { return *ip++; }

  inline EvaValue getConst() { return co->constants[readByte()]; }

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

  std::unique_ptr<syntax::EvaParser> parser;

  std::unique_ptr<EvaCompiler> compiler;

  CodeObject* co;

  uint8_t* ip;

  std::array<EvaValue, STACK_LIMIT> stack;

  EvaValue* sp;
};

#endif  // EVAVM_H