#ifndef OPCODE_H
#define OPCODE_H

#include <cstdint>

enum class OpCode : uint8_t {
  Halt = 0x00,
  Const = 0x01,
  Add = 0x02,
  Sub = 0x03,
  Mul = 0x04,
  Div = 0x05,
  Concat = 0x06,
  Compare = 0x07,
};

inline uint8_t to_uint8(OpCode op) { return static_cast<uint8_t>(op); }
inline OpCode to_opcode(uint8_t code) { return static_cast<OpCode>(code); }

#endif  // OPCODE_H
