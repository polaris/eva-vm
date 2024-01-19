#include "EvaCompiler.h"

std::map<std::string, uint8_t> EvaCompiler::compareOps = {
    {"<", 0}, {">", 1}, {"==", 2}, {">=", 3}, {"<=", 4}, {"!=", 5},
};
