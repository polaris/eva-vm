#ifndef UNKNOWNOPCODE_H
#define UNKNOWNOPCODE_H

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

#include "OpCode.h"

class UnknownOpcode : public std::runtime_error {
 public:
  UnknownOpcode(OpCode code) : std::runtime_error(createErrorMessage(code)) {}

 private:
  static std::string createErrorMessage(OpCode code) {
    std::ostringstream oss;
    oss << "Unknown opcode: 0x" << std::hex << std::setfill('0') << std::setw(2)
        << static_cast<int>(code);
    return oss.str();
  }
};

#endif  // UNKNOWNOPCODE_H
