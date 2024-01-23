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
  JmpIfFalse = 0x08,
  Jmp = 0x09,
};

inline uint8_t to_uint8(OpCode op) { return static_cast<uint8_t>(op); }
inline OpCode to_opcode(uint8_t code) { return static_cast<OpCode>(code); }
inline std::string opcodeToString(OpCode opcode) {
  switch (opcode) {
    case OpCode::Halt:
      return "Halt";
    case OpCode::Const:
      return "Const";
    case OpCode::Add:
      return "Add";
    case OpCode::Sub:
      return "Sub";
    case OpCode::Mul:
      return "Mul";
    case OpCode::Div:
      return "Div";
    case OpCode::Concat:
      return "Concat";
    case OpCode::Compare:
      return "Compare";
    case OpCode::JmpIfFalse:
      return "JmpIfFalse";
    case OpCode::Jmp:
      return "Jmp";
  }
}

#endif  // OPCODE_H
