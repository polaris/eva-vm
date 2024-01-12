#ifndef EVAVALUE_H
#define EVAVALUE_H

enum class EvaValueType { Number };

struct EvaValue {
  EvaValueType type;
  union {
    double number;
  };
};

inline EvaValue Number(double value) {
  return {.type = EvaValueType::Number, .number = value};
}

#endif  // EVAVALUE_H