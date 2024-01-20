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

TEST_CASE("Subtract number expression", "[exec]") {
  EvaVM vm;
  const auto result = vm.exec(R"( (- 20 22) )");
  REQUIRE(asNumber(result) == -2);
}

TEST_CASE("Multiply number expression", "[exec]") {
  EvaVM vm;
  const auto result = vm.exec(R"( (* 20 22) )");
  REQUIRE(asNumber(result) == 440);
}

TEST_CASE("Divide number expression", "[exec]") {
  EvaVM vm;
  const auto result = vm.exec(R"( (/ 20 4) )");
  REQUIRE(asNumber(result) == 5);
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

TEST_CASE("Less than expression", "[exec]") {
  EvaVM vm;

  const auto result1 = vm.exec(R"( (< 20 4) )");
  REQUIRE(asBoolean(result1) == false);

  const auto result2 = vm.exec(R"( (< 4 20) )");
  REQUIRE(asBoolean(result2) == true);
}

TEST_CASE("Greater than expression", "[exec]") {
  EvaVM vm;

  const auto result1 = vm.exec(R"( (> 20 4) )");
  REQUIRE(asBoolean(result1) == true);

  const auto result2 = vm.exec(R"( (> 4 20) )");
  REQUIRE(asBoolean(result2) == false);
}

TEST_CASE("Equality expression", "[exec]") {
  EvaVM vm;

  const auto result1 = vm.exec(R"( (== 20 4) )");
  REQUIRE(asBoolean(result1) == false);

  const auto result2 = vm.exec(R"( (== 4 4) )");
  REQUIRE(asBoolean(result2) == true);
}

TEST_CASE("Greater than or equal expression", "[exec]") {
  EvaVM vm;

  const auto result1 = vm.exec(R"( (>= 20 4) )");
  REQUIRE(asBoolean(result1) == true);

  const auto result2 = vm.exec(R"( (>= 4 20) )");
  REQUIRE(asBoolean(result2) == false);

  const auto result3 = vm.exec(R"( (>= 20 20) )");
  REQUIRE(asBoolean(result3) == true);
}

TEST_CASE("Less than or equal expression", "[exec]") {
  EvaVM vm;

  const auto result1 = vm.exec(R"( (<= 20 4) )");
  REQUIRE(asBoolean(result1) == false);

  const auto result2 = vm.exec(R"( (<= 4 20) )");
  REQUIRE(asBoolean(result2) == true);

  const auto result3 = vm.exec(R"( (<= 4 4) )");
  REQUIRE(asBoolean(result3) == true);
}

TEST_CASE("Inequality expression", "[exec]") {
  EvaVM vm;

  const auto result1 = vm.exec(R"( (!= 20 4) )");
  REQUIRE(asBoolean(result1) == true);

  const auto result2 = vm.exec(R"( (!= 4 4) )");
  REQUIRE(asBoolean(result2) == false);
}
