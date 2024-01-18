#ifndef EVAVALUE_H
#define EVAVALUE_H

#include <ostream>
#include <string>

#include "UnknownType.h"

enum class EvaValueType { Boolean, Number, Object };

struct Object;

struct EvaValue {
  EvaValueType type;
  union {
    bool boolean;
    double number;
    Object* object;
  };
};

enum class ObjectType { String, Code };

struct Object {
  Object(ObjectType t) : type(t) {}
  ObjectType type;

  virtual void print(std::ostream& os) = 0;
};

struct StringObject : public Object {
  StringObject(const std::string& s) : Object(ObjectType::String), string(s) {}
  virtual ~StringObject() {}
  std::string string;

  void print(std::ostream& os) override { os << string; }
};

struct CodeObject : public Object {
  CodeObject(const std::string& name) : Object(ObjectType::Code), name(name) {}
  virtual ~CodeObject() {}

  void print(std::ostream& os) override { os << name; }

  std::string name;
  std::vector<uint8_t> code;
  std::vector<EvaValue> constants;
};

inline EvaValue Boolean(bool value) {
  return {.type = EvaValueType::Boolean, .boolean = value};
}

inline bool isBoolean(const EvaValue& value) {
  return value.type == EvaValueType::Boolean;
}

inline bool asBoolean(const EvaValue& value) { return value.boolean; }

inline EvaValue Number(double value) {
  return {.type = EvaValueType::Number, .number = value};
}

inline bool isNumber(const EvaValue& value) {
  return value.type == EvaValueType::Number;
}

inline double asNumber(const EvaValue& value) { return value.number; }

inline Object* asObject(const EvaValue& value) { return value.object; }

inline bool isObject(const EvaValue& value) {
  return value.type == EvaValueType::Object;
}

inline bool isObjectType(const EvaValue& value, ObjectType type) {
  return isObject(value) && asObject(value)->type == type;
}

inline EvaValue allocString(const std::string& value) {
  return {.type = EvaValueType::Object, .object = new StringObject(value)};
}

inline bool isString(const EvaValue& value) {
  return isObjectType(value, ObjectType::String);
}

inline void DeallocString(const EvaValue& value) {
  if (isString(value)) {
    delete ((StringObject*)value.object);
  }
}

inline StringObject* asString(const EvaValue& value) {
  return (StringObject*)value.object;
}

inline std::string asCppString(const EvaValue& value) {
  return (asString(value))->string;
}

inline EvaValue allocCode(const std::string& name) {
  return {.type = EvaValueType::Object, .object = new CodeObject(name)};
}

inline bool isCode(const EvaValue& value) {
  return isObjectType(value, ObjectType::Code);
}

inline CodeObject* asCode(const EvaValue& value) {
  return (CodeObject*)value.object;
}

static std::string evaValueToTypeString(const EvaValue& evaValue) {
  if (isBoolean(evaValue)) {
    return "BOOLEAN";
  } else if (isNumber(evaValue)) {
    return "NUMBER";
  } else if (isString(evaValue)) {
    return "STRING";
  } else if (isCode(evaValue)) {
    return "CODE";
  } else {
    throw UnknownType(static_cast<int>(evaValue.type));
  }
}

static std::string evaValueToConstantString(const EvaValue& evaValue) {
  std::stringstream ss;
  if (isBoolean(evaValue)) {
    ss << ((asBoolean(evaValue) == true) ? "true" : "false");
  } else if (isNumber(evaValue)) {
    ss << asNumber(evaValue);
  } else if (isString(evaValue)) {
    ss << '"' << asCppString(evaValue) << '"';
  } else if (isCode(evaValue)) {
    auto code = asCode(evaValue);
    ss << "code " << code << ": " << code->name;
  } else {
    throw UnknownType(static_cast<int>(evaValue.type));
  }
  return ss.str();
}

static std::ostream& operator<<(std::ostream& os, const EvaValue& evaValue) {
  os << "EvaValue(" << evaValueToTypeString(evaValue)
     << "): " << evaValueToConstantString(evaValue);
  return os;
}

#endif  // EVAVALUE_H