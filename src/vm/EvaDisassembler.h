#ifndef EVADISASSEMBLER_H
#define EVADISASSEMBLER_H

#include <iostream>
#include <sstream>

#include "EvaValue.h"
#include "OpCode.h"

class EvaDisassember {
 public:
  void disassemble(CodeObject* co) {
    std::cout << "\n-------------- Disassembly: " << co->name
              << " --------------\n\n";
    size_t offset = 0;
    while (offset < co->code.size()) {
      offset = disassembleInstruction(co, offset);
      std::cout << "\n";
    }
  }

 private:
  size_t disassembleInstruction(CodeObject* co, size_t offset) {
    std::ios_base::fmtflags f(std::cout.flags());
    std::cout << std::uppercase << std::hex << std::setfill('0') << std::setw(4)
              << offset << "    ";
    const auto opcode = to_opcode(co->code[offset]);
    switch (opcode) {
      case OpCode::Halt:
        return disassembleSimple(co, opcode, offset);
      case OpCode::Const:
        return disassembleConst(co, offset);
      case OpCode::Add:
      case OpCode::Sub:
      case OpCode::Mul:
      case OpCode::Div:
      case OpCode::Concat:
        return disassembleSimple(co, opcode, offset);
      case OpCode::Compare:
        return disassembleCompare(co, offset);
      case OpCode::JmpIfFalse:
      case OpCode::Jmp:
        return disassembleJump(co, opcode, offset);
      default:
        throw std::runtime_error("Invalid instruction");
    }

    std::cout.flags(f);
  }

  size_t disassembleSimple(CodeObject* co, OpCode opcode, size_t offset) {
    dumpBytes(co, offset, 1);
    printOpCode(opcode);
    return offset + 1;
  }

  size_t disassembleConst(CodeObject* co, size_t offset) {
    dumpBytes(co, offset, 2);
    printOpCode(OpCode::Const);
    const auto constIndex = co->code[offset + 1];
    std::cout << static_cast<int>(constIndex) << " ("
              << evaValueToConstantString(co->constants[constIndex]) << ")";
    return offset + 2;
  }

  size_t disassembleCompare(CodeObject* co, size_t offset) {
    static std::array<std::string, 6> inverseCompareOps = {
        "<", ">", "==", ">=", "<=", "!=",
    };
    dumpBytes(co, offset, 2);
    printOpCode(OpCode::Compare);
    const auto compareOp = co->code[offset + 1];
    std::cout << static_cast<int>(compareOp) << " ("
              << inverseCompareOps[compareOp] << ")";
    return offset + 2;
  }

  size_t disassembleJump(CodeObject* co, OpCode opcode, size_t offset) {
    std::ios_base::fmtflags f(std::cout.flags());
    dumpBytes(co, offset, 3);
    printOpCode(opcode);
    const uint16_t address = readWordAtOffset(co, offset + 1);
    std::cout << std::uppercase << std::hex << std::setfill('0') << std::setw(4)
              << static_cast<int>(address) << " ";
    std::cout.flags(f);
    return offset + 3;
  }

  void dumpBytes(CodeObject* co, size_t offset, size_t count) {
    std::ios_base::fmtflags f(std::cout.flags());
    std::stringstream ss;
    for (size_t i = 0; i < count; i++) {
      ss << std::uppercase << std::hex << std::setfill('0') << std::setw(2)
         << (((int)co->code[offset + i]) & 0xff) << " ";
    }
    std::cout << std::left << std::setfill(' ') << std::setw(12) << ss.str();
    std::cout.flags(f);
  }

  void printOpCode(OpCode opcode) {
    std::ios_base::fmtflags f(std::cout.flags());
    std::cout << std::left << std::setfill(' ') << std::setw(20)
              << opcodeToString(opcode) << " ";
    std::cout.flags(f);
  }

  uint16_t readWordAtOffset(CodeObject* co, size_t offset) {
    return static_cast<uint16_t>((co->code[offset] << 8) | co->code[offset + 1]);
  }
};

#endif
