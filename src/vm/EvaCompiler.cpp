#include "EvaCompiler.h"

#include <algorithm>

std::map<std::string, uint8_t> EvaCompiler::compareOps = {
    {"<", 0}, {">", 1}, {"==", 2}, {">=", 3}, {"<=", 4}, {"!=", 5},
};

EvaCompiler::EvaCompiler(std::shared_ptr<Global> global)
    : global{global}, disassembler{global} {}

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

template <typename T>
void EvaCompiler::genConst(const T& value) {
  emit(to_uint8(OpCode::Const));
  emit(constIdx(value));
}

template <typename T>
size_t EvaCompiler::constIdx(const T& value) {
  const EvaValue evaValue(value);
  const auto it =
      std::find(co->constants.begin(), co->constants.end(), evaValue);
  if (it != co->constants.end()) {
    return std::distance(co->constants.begin(), it);
  }
  co->constants.push_back(evaValue);
  return co->constants.size() - 1;
}

template <>
size_t EvaCompiler::constIdx<int>(const int& value) {
  return constIdx(static_cast<double>(value));
}

void EvaCompiler::genSymbol(const Exp& exp) {
  if (exp.string == "true" || exp.string == "false") {
    emit(to_uint8(OpCode::Const));
    emit(constIdx(exp.string == "true" ? true : false));
  } else {
    const auto name = exp.string;
    const auto localIndex = co->getLocalIndex(name);
    if (localIndex != -1) {
      emit(to_uint8(OpCode::GetLocal));
      emit(localIndex);
    } else {
      if (!global->exists(name)) {
        throw std::runtime_error("Reference error");
      }
      emit(to_uint8(OpCode::GetGlobal));
      emit(global->getGlobalIndex(name));
    }
  }
}

void EvaCompiler::genList(const Exp& exp) {
  const auto tag = exp.list[0];
  if (tag.type == ExpType::SYMBOL) {
    const auto op = tag.string;
    if (op == "+") {
      if (exp.list[1].type == ExpType::STRING) {
        genBinaryOp(exp, OpCode::Concat);
      } else {
        genBinaryOp(exp, OpCode::Add);
      }
    } else if (op == "-") {
      genBinaryOp(exp, OpCode::Sub);
    } else if (op == "*") {
      genBinaryOp(exp, OpCode::Mul);
    } else if (op == "/") {
      genBinaryOp(exp, OpCode::Div);
    } else if (compareOps.count(op) != 0) {
      genCompareOp(exp, op);
    } else if (op == "if") {
      genIfOp(exp);
    } else if (op == "var") {
      genVar(exp);
    } else if (op == "set") {
      genSet(exp);
    } else if (op == "begin") {
      genBegin(exp);
    }
  }
}

void EvaCompiler::genBinaryOp(const Exp& exp, OpCode oc) {
  gen(exp.list[1]);
  gen(exp.list[2]);
  emit(to_uint8(oc));
}

void EvaCompiler::genCompareOp(const Exp& exp, const std::string& op) {
  gen(exp.list[1]);
  gen(exp.list[2]);
  emit(to_uint8(OpCode::Compare));
  emit(compareOps[op]);
}

void EvaCompiler::genIfOp(const Exp& exp) {
  gen(exp.list[1]);
  emit(to_uint8(OpCode::JmpIfFalse));
  emit(0);
  emit(0);
  const auto elseJmpAddr = getOffset() - 2;
  gen(exp.list[2]);
  emit(to_uint8(OpCode::Jmp));
  emit(0);
  emit(0);
  const auto endAddr = getOffset() - 2;
  const auto elseBranchAddr = getOffset();
  patchJumpAddress(elseJmpAddr, elseBranchAddr);
  if (exp.list.size() == 4) {
    gen(exp.list[3]);
  }
  const auto endBranchAddr = getOffset();
  patchJumpAddress(endAddr, endBranchAddr);
}

void EvaCompiler::genVar(const Exp& exp) {
  const auto name = exp.list[1].string;
  gen(exp.list[2]);
  if (isGlobalScope()) {
    global->define(name);
    emit(to_uint8(OpCode::SetGlobal));
    emit(global->getGlobalIndex(name));
  } else {
    co->addLocal(name);
    emit(to_uint8(OpCode::SetLocal));
    emit(co->getLocalIndex(name));
  }
}

void EvaCompiler::genSet(const Exp& exp) {
  const auto name = exp.list[1].string;
  gen(exp.list[2]);
  const auto localIndex = co->getLocalIndex(name);
  if (localIndex == -1) {
    emit(to_uint8(OpCode::SetLocal));
    emit(localIndex);
  } else {
    const auto globalIndex = global->getGlobalIndex(name);
    if (globalIndex == -1) {
      throw std::runtime_error("Reference error");
    }
    emit(to_uint8(OpCode::SetGlobal));
    emit(globalIndex);
  }
}

void EvaCompiler::genBegin(const Exp& exp) {
  scopeEnter();
  for (size_t i = 1; i < exp.list.size(); i++) {
    const bool isLast = (i == (exp.list.size() - 1));
    const bool isLocalDecl = isDeclaration(exp.list[1]) && !isGlobalScope();
    gen(exp.list[i]);
    if (!isLast && !isLocalDecl) {
      emit(to_uint8(OpCode::Pop));
    }
  }
  scopeExit();
}

size_t EvaCompiler::getOffset() const { return co->code.size(); }

void EvaCompiler::writeByteAtOffset(size_t offset, uint8_t value) {
  co->code[offset] = value;
}

void EvaCompiler::patchJumpAddress(size_t offset, uint16_t value) {
  writeByteAtOffset(offset, (value >> 8) & 0xff);
  writeByteAtOffset(offset + 1, value & 0xff);
}

void EvaCompiler::emit(uint8_t oc) { co->code.push_back(oc); }
