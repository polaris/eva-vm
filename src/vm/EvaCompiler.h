#ifndef EVACOMPILER_H
#define EVACOMPILER_H

#include "EvaParser.h"
#include "EvaValue.h"
#include "OpCode.h"

class EvaCompiler {
 public:
  EvaCompiler() {}

  CodeObject* compile(const Exp& exp) {
    co = asCode(allocCode("main"));

    gen(exp);
    emit(to_uint8(OpCode::OP_HALT));

    return co;
  }

 private:
  void gen(const Exp& exp) {
    switch (exp.type) {
      case ExpType::NUMBER:
        emitConst(exp.number);
        break;
      case ExpType::STRING:
        emitConst(exp.string);
        break;
    }
  }

  template <typename T>
  void emitConst(const T& value) {
    emit(to_uint8(OpCode::OP_CONST));
    emit(constIdx(value));
  }

  size_t constIdx(double value) {
    for (std::size_t i = 0; i < co->constants.size(); ++i) {
      if (isNumber(co->constants[i]) && asNumber(co->constants[i]) == value) {
        return i;
      }
    }
    co->constants.push_back(Number(value));
    return co->constants.size() - 1;
  }

  size_t constIdx(const std::string& value) {
    for (std::size_t i = 0; i < co->constants.size(); ++i) {
      if (isString(co->constants[i]) &&
          asCppString(co->constants[i]) == value) {
        return i;
      }
    }
    co->constants.push_back(allocString(value));
    return co->constants.size() - 1;
  }

  void emit(uint8_t oc) { co->code.push_back(oc); }

  CodeObject* co;
};

#endif  // EVACOMPILER_H