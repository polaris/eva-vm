#include <iostream>

#include "vm/EvaVM.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
  try {
    EvaVM vm;
    const auto result = vm.exec(R"(42)");
    std::cout << asNumber(result) << "\n";
    std::cout << "All done!\n";
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "\n";
  }
}
