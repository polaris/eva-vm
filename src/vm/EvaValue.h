#ifndef EVAVALUE_H
#define EVAVALUE_H

#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "UnknownType.h"

struct CodeObject;

using EvaValue = std::variant<bool, double, std::string, CodeObject*>;

struct CodeObject {
  CodeObject(const std::string& n) : name(n) {}
  std::string name;
  std::vector<uint8_t> code;
  std::vector<EvaValue> constants;
};

inline bool isBoolean(const EvaValue& value) {
  return std::holds_alternative<bool>(value);
}

inline bool asBoolean(const EvaValue& value) { return std::get<bool>(value); }

inline bool isNumber(const EvaValue& value) {
  return std::holds_alternative<double>(value);
}

inline double asNumber(const EvaValue& value) {
  return std::get<double>(value);
}

inline bool isString(const EvaValue& value) {
  return std::holds_alternative<std::string>(value);
}

inline std::string asString(const EvaValue& value) {
  return std::get<std::string>(value);
}

inline bool isCode(const EvaValue& value) {
  return std::holds_alternative<CodeObject*>(value);
}

inline CodeObject* asCode(const EvaValue& value) {
  return std::get<CodeObject*>(value);
}

inline std::string evaValueToTypeString(const EvaValue& evaValue) {
  if (isBoolean(evaValue)) {
    return "BOOLEAN";
  } else if (isNumber(evaValue)) {
    return "NUMBER";
  } else if (isString(evaValue)) {
    return "STRING";
  } else if (isCode(evaValue)) {
    return "CODE";
  } else {
    throw UnknownType();
  }
}

static std::string evaValueToConstantString(const EvaValue& evaValue) {
  std::stringstream ss;
  if (isBoolean(evaValue)) {
    ss << (asBoolean(evaValue) ? "true" : "false");
  } else if (isNumber(evaValue)) {
    ss << asNumber(evaValue);
  } else if (isString(evaValue)) {
    ss << '"' << asString(evaValue) << '"';
  } else if (isCode(evaValue)) {
    auto codeObject = asCode(evaValue);
    ss << "code: " << codeObject->name;
  } else {
    throw UnknownType();
  }
  return ss.str();
}

inline std::ostream& operator<<(std::ostream& os, const EvaValue& evaValue) {
  os << "EvaValue(" << evaValueToTypeString(evaValue)
     << "): " << evaValueToConstantString(evaValue);
  return os;
}

#endif  // EVAVALUE_H
