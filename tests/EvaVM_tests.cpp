#include <catch2/catch_test_macros.hpp>

#include "../src/vm/EvaVM.h"

TEST_CASE("Code is executed", "[exec]") {
  EvaVM vm;
  const auto result = vm.exec(R"(42)");
  REQUIRE(result.number == 42);
}