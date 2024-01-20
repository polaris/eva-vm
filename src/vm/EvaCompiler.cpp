#include "EvaCompiler.h"

std::map<std::string, uint8_t> EvaCompiler::compareOps = {
    {"<", 0}, {">", 1}, {"==", 2}, {">=", 3}, {"<=", 4}, {"!=", 5},
};

EvaCompiler::EvaCompiler() {}

CodeObject* EvaCompiler::compile(const Exp& exp) {
  co = std::make_shared<CodeObject>("main");
  gen(exp);
  emit(to_uint8(OpCode::Halt));
  return co.get();
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
      genSymbol(exp);
      break;
    case ExpType::LIST:
      genList(exp);
      break;
  }
}

void EvaCompiler::genList(const Exp& exp) {
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
}

void EvaCompiler::genSymbol(const Exp& exp) {
  if (exp.string == "true" || exp.string == "false") {
    emit(to_uint8(OpCode::Const));
    emit(constIdx(exp.string == "true" ? true : false));
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

template <typename T>
size_t EvaCompiler::constIdx(const T& value) {
  const EvaValue evaValue(value);
  for (std::size_t i = 0; i < co->constants.size(); ++i) {
    if (co->constants[i] == evaValue) {
      return i;
    }
  }
  co->constants.push_back(evaValue);
  return co->constants.size() - 1;
}

template <>
size_t EvaCompiler::constIdx<int>(const int& value) {
  return constIdx(static_cast<double>(value));
}

void EvaCompiler::emit(uint8_t oc) { co->code.push_back(oc); }
