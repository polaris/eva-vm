#ifndef STACKOVERFLOW_H
#define STACKOVERFLOW_H

#include <stdexcept>

class StackOverflow : public std::runtime_error {
 public:
  StackOverflow() : std::runtime_error("Stack overflow") {}
};

#endif  // STACKOVERFLOW_H