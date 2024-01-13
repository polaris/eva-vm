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

enum class ObjectType { String };

struct Object {
  Object(ObjectType t) : type(t) {}
  ObjectType type;
};

struct StringObject : public Object {
  StringObject(const std::string& s) : Object(ObjectType::String), string(s) {}
  std::string string;
};

inline EvaValue Number(double value) {
  return {.type = EvaValueType::Number, .number = value};
}

inline EvaValue AllocString(const std::string& value) {
  return {.type = EvaValueType::Object, .object = new StringObject(value)};
}

inline double asNumber(const EvaValue& value) { return value.number; }

inline Object* asObject(const EvaValue& value) { return value.object; }

inline StringObject* asString(const EvaValue& value) {
  return (StringObject*)value.object;
}

inline std::string asCppString(const EvaValue& value) {
  return (asString(value))->string;
}

inline bool isNumber(const EvaValue& value) {
  return value.type == EvaValueType::Number;
}

inline bool isObject(const EvaValue& value) {
  return value.type == EvaValueType::Object;
}

inline bool isObjectType(const EvaValue& value, ObjectType type) {
  return isObject(value) && asObject(value)->type == type;
}

inline bool isString(const EvaValue& value) {
  return isObjectType(value, ObjectType::String);
}

inline void DeallocString(const EvaValue& value) {
  if (isString(value)) {
    delete ((StringObject*)value.object);
  }
}

#endif  // EVAVALUE_H