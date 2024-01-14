#include <catch2/catch_test_macros.hpp>

#include "../src/vm/EvaVM.h"

TEST_CASE("Number expression", "[exec]") {
  EvaVM vm;
  const auto result = vm.exec(R"(42)");
  REQUIRE(asNumber(result) == 42);
}

TEST_CASE("String expression", "[exec]") {
  EvaVM vm;
  const auto result = vm.exec(R"("hello")");
  REQUIRE(asCppString(result) == "hello");
}
