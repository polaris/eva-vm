#ifndef EVAVALUE_H
#define EVAVALUE_H

#include <string>

enum class EvaValueType { Number, Object };

struct Object;

struct EvaValue {
  EvaValueType type;
  union {
    double number;
    Object* object;
  };
};

enum class ObjectType { String, Code };

struct Object {
  Object(ObjectType t) : type(t) {}
  ObjectType type;
};

struct StringObject : public Object {
  StringObject(const std::string& s) : Object(ObjectType::String), string(s) {}
  std::string string;
};

struct CodeObject : public Object {
  CodeObject(const std::string& name) : Object(ObjectType::Code), name(name) {}

  std::string name;
  std::vector<uint8_t> code;
  std::vector<EvaValue> constants;
};

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

#endif  // EVAVALUE_H