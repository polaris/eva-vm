#ifndef UNKNOWNTYPE_H
#define UNKNOWNTYPE_H

#include <iomanip>
#include <stdexcept>

class UnknownType : public std::runtime_error {
 public:
  UnknownType() : std::runtime_error("Unknown Eva type") {}
};

#endif  // UNKNOWNTYPE_H
