#ifndef EMPTYSTACK_H
#define EMPTYSTACK_H

#include <stdexcept>

class EmptyStack : public std::runtime_error {
 public:
  EmptyStack() : std::runtime_error("Empty stack") {}
};

#endif  // EMPTYSTACK_H
