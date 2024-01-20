#ifndef EVACOMPILER_H
#define EVACOMPILER_H

#include <map>
#include <memory>
#include <string>

#include "EvaParser.h"
#include "EvaValue.h"
#include "OpCode.h"

class EvaCompiler {
 public:
  EvaCompiler();

  CodeObject* compile(const Exp& exp);

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

  void genBinaryOp(const Exp& exp, OpCode oc);

  void genCompareOp(const Exp& exp, const std::string& op);

  void emit(uint8_t oc);

  std::shared_ptr<CodeObject> co;

  static std::map<std::string, uint8_t> compareOps;
};

#endif  // EVACOMPILER_H
