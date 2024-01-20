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
  REQUIRE(asString(result) == "hello");
}

TEST_CASE("Concatenate string expression", "[exec]") {
  EvaVM vm;
  const auto result = vm.exec(R"( (+ "Hello, " "world!") )");
  REQUIRE(asString(result) == "Hello, world!");
}

TEST_CASE("Boolean expression - true", "[exec]") {
  EvaVM vm;
  const auto result = vm.exec(R"(true)");
  REQUIRE(asBoolean(result) == true);
}

TEST_CASE("Boolean expression - false", "[exec]") {
  EvaVM vm;
  const auto result = vm.exec(R"(false)");
  REQUIRE(asBoolean(result) == false);
}
