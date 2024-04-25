#include <iostream>

#include "vm/EvaVM.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
  try {
    EvaVM vm;
    const auto result = vm.exec(R"(

      (var t (begin
        (var s (+ 1 2))
        (var u (+ s 2))
      ))
      (+ t 1)

    )");
    std::cout << result << "\n";
    std::cout << "All done!\n";
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "\n";
  }
}
