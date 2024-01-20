#ifndef EVACOMPILER_H
#define EVACOMPILER_H

#include <map>
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

  void genCompareOp(const Exp& exp, const std::string& op);

  void genBinaryOp(const Exp& exp, OpCode oc);

  template <typename T>
  void genConst(const T& value);

  size_t constIdx(bool value);

  size_t constIdx(int value);

  size_t constIdx(double value);

  size_t constIdx(const std::string& value);

  void emit(uint8_t oc);

  CodeObject* co;

  static std::map<std::string, uint8_t> compareOps;
};

#endif  // EVACOMPILER_H