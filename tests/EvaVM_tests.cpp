#include <catch2/catch_test_macros.hpp>

#include "../src/vm/EvaVM.h"

TEST_CASE("Number expression", "[exec]") {
  EvaVM vm;
  const auto result = vm.exec(R"(42)");
  REQUIRE(asNumber(result) == 42);
}

TEST_CASE("Add number expression", "[exec]") {
  EvaVM vm;
  const auto result = vm.exec(R"( (+ 20 22) )");
  REQUIRE(asNumber(result) == 42);
}

TEST_CASE("String expression", "[exec]") {
  EvaVM vm;
  const auto result = vm.exec(R"("hello")");
  REQUIRE(asCppString(result) == "hello");
}

TEST_CASE("Concatenate string expression", "[exec]") {
  EvaVM vm;
  const auto result = vm.exec(R"( (+ "Hello, " "world!") )");
  REQUIRE(asCppString(result) == "Hello, world!");
}
