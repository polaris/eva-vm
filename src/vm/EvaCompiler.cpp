#include "EvaCompiler.h"

std::map<std::string, uint8_t> EvaCompiler::compareOps = {
    {"<", 0}, {">", 1}, {"==", 2}, {">=", 3}, {"<=", 4}, {"!=", 5},
};

EvaCompiler::EvaCompiler() {}

CodeObject* EvaCompiler::compile(const Exp& exp) {
  co = asCode(allocCode("main"));
  gen(exp);
  emit(to_uint8(OpCode::Halt));
  return co;
}

void EvaCompiler::gen(const Exp& exp) {
  switch (exp.type) {
    case ExpType::NUMBER:
      genConst(exp.number);
      break;
    case ExpType::STRING:
      genConst(exp.string);
      break;
    case ExpType::SYMBOL:
      if (exp.string == "true" || exp.string == "false") {
        emit(to_uint8(OpCode::Const));
        emit(constIdx(exp.string == "true" ? true : false));
      }
      break;
    case ExpType::LIST:
      const auto tag = exp.list[0];
      if (tag.type == ExpType::SYMBOL) {
        const auto op = tag.string;
        if (op == "+") {
          if (exp.list[1].type == ExpType::NUMBER) {
            genBinaryOp(exp, OpCode::Add);
          } else {
            genBinaryOp(exp, OpCode::Concat);
          }
        } else if (op == "-") {
          genBinaryOp(exp, OpCode::Sub);
        } else if (op == "*") {
          genBinaryOp(exp, OpCode::Mul);
        } else if (op == "/") {
          genBinaryOp(exp, OpCode::Div);
        } else if (compareOps.count(op) != 0) {
          genCompareOp(exp, op);
        }
      }
      break;
  }
}

void EvaCompiler::genCompareOp(const Exp& exp, const std::string& op) {
  gen(exp.list[1]);
  gen(exp.list[2]);
  emit(to_uint8(OpCode::Compare));
  emit(compareOps[op]);
}

void EvaCompiler::genBinaryOp(const Exp& exp, OpCode oc) {
  gen(exp.list[1]);
  gen(exp.list[2]);
  emit(to_uint8(oc));
}

template <typename T>
void EvaCompiler::genConst(const T& value) {
  emit(to_uint8(OpCode::Const));
  emit(constIdx(value));
}

size_t EvaCompiler::constIdx(bool value) {
  for (std::size_t i = 0; i < co->constants.size(); ++i) {
    if (isBoolean(co->constants[i]) && asBoolean(co->constants[i]) == value) {
      return i;
    }
  }
  co->constants.push_back(Boolean(value));
  return co->constants.size() - 1;
}

size_t EvaCompiler::constIdx(int value) {
  for (std::size_t i = 0; i < co->constants.size(); ++i) {
    if (isNumber(co->constants[i]) &&
        static_cast<int>(asNumber(co->constants[i])) == value) {
      return i;
    }
  }
  co->constants.push_back(Number(static_cast<double>(value)));
  return co->constants.size() - 1;
}

size_t EvaCompiler::constIdx(double value) {
  for (std::size_t i = 0; i < co->constants.size(); ++i) {
    if (isNumber(co->constants[i]) && asNumber(co->constants[i]) == value) {
      return i;
    }
  }
  co->constants.push_back(Number(value));
  return co->constants.size() - 1;
}

size_t EvaCompiler::constIdx(const std::string& value) {
  for (std::size_t i = 0; i < co->constants.size(); ++i) {
    if (isString(co->constants[i]) && asCppString(co->constants[i]) == value) {
      return i;
    }
  }
  co->constants.push_back(allocString(value));
  return co->constants.size() - 1;
}

void EvaCompiler::emit(uint8_t oc) { co->code.push_back(oc); }
