#ifndef UNKNOWNTYPE_H
#define UNKNOWNTYPE_H

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

class UnknownType : public std::runtime_error {
 public:
  UnknownType(int type) : std::runtime_error(createErrorMessage(type)) {}

 private:
  static std::string createErrorMessage(int type) {
    std::ostringstream oss;
    oss << "Unknown Eva type: " << type;
    return oss.str();
  }
};

#endif  // UNKNOWNTYPE_H