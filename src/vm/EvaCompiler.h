#ifndef EVACOMPILER_H
#define EVACOMPILER_H

#include <map>
#include <string>

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
        genConst(exp.number);
        break;
      case ExpType::STRING:
        genConst(exp.string);
        break;
      case ExpType::SYMBOL:
        if (exp.string == "true" || exp.string == "false") {
          emit(to_uint8(OpCode::OP_CONST));
          emit(constIdx(exp.string == "true" ? true : false));
        }
        break;
      case ExpType::LIST:
        const auto tag = exp.list[0];
        if (tag.type == ExpType::SYMBOL) {
          const auto op = tag.string;
          if (op == "+") {
            if (exp.list[1].type == ExpType::NUMBER) {
              genBinaryOp(exp, OpCode::OP_ADD);
            } else {
              genBinaryOp(exp, OpCode::OP_CONCAT);
            }
          } else if (op == "-") {
            genBinaryOp(exp, OpCode::OP_SUB);
          } else if (op == "*") {
            genBinaryOp(exp, OpCode::OP_MUL);
          } else if (op == "/") {
            genBinaryOp(exp, OpCode::OP_DIV);
          } else if (compareOps.count(op) != 0) {
            gen(exp.list[1]);
            gen(exp.list[2]);
            emit(to_uint8(OpCode::OP_COMPARE));
            emit(compareOps[op]);
          }
        }
        break;
    }
  }

  void genBinaryOp(const Exp& exp, OpCode oc) {
    gen(exp.list[1]);
    gen(exp.list[2]);
    emit(to_uint8(oc));
  }

  template <typename T>
  void genConst(const T& value) {
    emit(to_uint8(OpCode::OP_CONST));
    emit(constIdx(value));
  }

  size_t constIdx(bool value) {
    for (std::size_t i = 0; i < co->constants.size(); ++i) {
      if (isBoolean(co->constants[i]) && asBoolean(co->constants[i]) == value) {
        return i;
      }
    }
    co->constants.push_back(Boolean(value));
    return co->constants.size() - 1;
  }

  size_t constIdx(int value) {
    for (std::size_t i = 0; i < co->constants.size(); ++i) {
      if (isNumber(co->constants[i]) &&
          static_cast<int>(asNumber(co->constants[i])) == value) {
        return i;
      }
    }
    co->constants.push_back(Number(static_cast<double>(value)));
    return co->constants.size() - 1;
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

  static std::map<std::string, uint8_t> compareOps;
};

#endif  // EVACOMPILER_H