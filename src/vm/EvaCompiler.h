#ifndef EVACOMPILER_H
#define EVACOMPILER_H

#include <algorithm>
#include <map>
#include <memory>
#include <string>

#include "EvaDisassembler.h"
#include "EvaParser.h"
#include "EvaValue.h"
#include "Global.h"
#include "OpCode.h"

class EvaCompiler {
 public:
  EvaCompiler(std::shared_ptr<Global> global);

  CodeObject* compile(const Exp& exp);

  void disassembleByteCode() { disassembler.disassemble(co.get()); }

 private:
  void gen(const Exp& exp);

  template <typename T>
  void genConst(const T& value);

  template <typename T>
  size_t constIdx(const T& value);

  template <>
  size_t constIdx<int>(const int& value);

  void genSymbol(const Exp& exp);

  void genList(const Exp& exp);

  void genBegin(const Exp& exp);

  void genVar(const Exp& exp);

  void genSet(const Exp& exp);

  void genBinaryOp(const Exp& exp, OpCode oc);

  void genCompareOp(const Exp& exp, const std::string& op);

  void genIfOp(const Exp& exp);

  void scopeEnter() { co->scopeLevel++; }

  void scopeExit() {
    const size_t varCount = getVarsCountOnScopeExit();
    if (varCount > 0) {
      emit(to_uint8(OpCode::ScopeExit));
      std::cout << "----2 " << varCount << std::endl;
      emit(varCount);
    }
    co->scopeLevel--;
  }

  bool isGlobalScope() { return co->scopeLevel == 0; }

  bool isDeclaration(const Exp& exp) { return isVarDeclaration(exp); }

  bool isVarDeclaration(const Exp& exp) { return isTaggedList(exp, "var"); }

  bool isTaggedList(const Exp& exp, const std::string& tag) {
    return exp.type == ExpType::LIST && exp.list[0].type == ExpType::SYMBOL &&
           exp.list[0].string == tag;
  }

  size_t getVarsCountOnScopeExit() {
    size_t varCount = 0;
    if (!co->locals.empty()) {
      auto scopeEnd = std::find_if(
          co->locals.rbegin(), co->locals.rend(), [&](const auto& localVar) {
            return localVar.scopeLevel != co->scopeLevel;
          });
      varCount = std::distance(scopeEnd, co->locals.rend());
      co->locals.erase(scopeEnd.base(), co->locals.end());
    }
    return varCount;
  }

  size_t getOffset() const;

  void writeByteAtOffset(size_t offset, uint8_t value);

  void patchJumpAddress(size_t offset, uint16_t value);

  void emit(uint8_t oc);

  std::shared_ptr<Global> global;

  std::shared_ptr<CodeObject> co;

  EvaDisassember disassembler;

  static std::map<std::string, uint8_t> compareOps;
};

#endif  // EVACOMPILER_H
