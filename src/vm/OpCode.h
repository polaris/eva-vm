#ifndef OPCODE_H
#define OPCODE_H

#include <cstdint>

enum class OpCode : uint8_t {
  OP_HALT = 0x00,
  OP_CONST = 0x01,
  OP_ADD = 0x02,
  OP_SUB = 0x03,
  OP_MUL = 0x04,
  OP_DIV = 0x05,
  OP_CONCAT = 0x06,
  OP_COMPARE = 0x07,
};

inline uint8_t to_uint8(OpCode op) { return static_cast<uint8_t>(op); }
inline OpCode to_opcode(uint8_t code) { return static_cast<OpCode>(code); }

#endif  // OPCODE_H
