#ifndef EVAVM_H
#define EVAVM_H

#include <array>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "EmptyStack.h"
#include "EvaCompiler.h"
#include "EvaParser.h"
#include "EvaValue.h"
#include "Global.h"
#include "StackOverflow.h"

#define STACK_LIMIT 512

// Eva Virtual Machine
class EvaVM {
 public:
  EvaVM();

  EvaValue exec(const std::string& program);

 private:
  void setGlobalVariables();

  EvaValue eval();

  void handleJmpIfFalse();

  void handleJmp();

  void handleGetGlobal();

  void handleSetGlobal();

  void handleGetLocal();

  void handleSetLocal();

  void handlePop();

  void handleScopeExit();

  void handleDefault(const OpCode& opcode);

  template <typename Operation>
  void binaryOp(Operation op) {
    const auto op2 = asNumber(pop());
    const auto op1 = asNumber(pop());
    push(op(op1, op2));
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
      const auto s1 = asString(op1);
      const auto s2 = asString(op2);
      compareValues(op, s1, s2);
    }
  }

  template <typename T>
  void compareValues(uint8_t op, const T& v1, const T& v2) {
    static const std::function<bool(T, T)> compareFuncs[] = {
        std::less<T>(),          std::greater<T>(),    std::equal_to<T>(),
        std::greater_equal<T>(), std::less_equal<T>(), std::not_equal_to<T>()};
    if (op >= std::size(compareFuncs)) {
      throw std::invalid_argument("Invalid comparison operation");
    }
    push(compareFuncs[op](v1, v2));
  }

  void concat() {
    const auto op2 = asString(pop());
    const auto op1 = asString(pop());
    push(EvaValue(op1 + op2));
  }

  inline uint8_t readByte() { return *ip++; }

  inline uint16_t readShort() {
    ip += 2;
    return static_cast<uint16_t>((ip[-2] << 8) | ip[-1]);
  }

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

  inline void popN(size_t count) {
    if (stack.size() == 0) {
      throw std::runtime_error("Stack empty");
    }
    sp -= count;
  }

  inline EvaValue peek(size_t offset = 0) {
    if (sp == stack.begin()) {
      throw EmptyStack();
    }
    return *(sp - 1 - offset);
  }

  uint8_t* toAddress(uint16_t index) { return &co->code[index]; }

  std::unique_ptr<syntax::EvaParser> parser;

  std::shared_ptr<Global> global;

  std::unique_ptr<EvaCompiler> compiler;

  CodeObject* co;

  uint8_t* ip;

  std::array<EvaValue, STACK_LIMIT> stack;

  EvaValue* sp;

  EvaValue* bp;
};

#endif  // EVAVM_H
