#include <catch2/catch_test_macros.hpp>

#include "../src/vm/EvaValue.h"

TEST_CASE("Number construction", "[EvaValue]") {
  const EvaValue n(1.0);
  REQUIRE(isNumber(n) == true);
  REQUIRE(asNumber(n) == 1.0);
}

TEST_CASE("Number accessor", "[EvaValue]") {
  const EvaValue n(1.0);
  REQUIRE(asNumber(n) == 1.0);
}

TEST_CASE("Number test", "[EvaValue]") {
  const EvaValue n(1.0);
  const EvaValue s("foobar");
  REQUIRE(isNumber(n) == true);
  REQUIRE(isNumber(s) == false);
}

TEST_CASE("String construction", "[EvaValue]") {
  const EvaValue s("foobar");
  REQUIRE(isString(s) == true);
  REQUIRE(asString(s) == "foobar");
}

TEST_CASE("String accessor", "[EvaValue]") {
  const EvaValue s("Hello, world!");
  REQUIRE(asString(s) == "Hello, world!");
}

TEST_CASE("String test", "[EvaValue]") {
  const EvaValue s("foobar");
  const EvaValue n(1.0);
  REQUIRE(isString(s) == true);
  REQUIRE(isString(n) == false);
}

TEST_CASE("Boolean construction", "[EvaValue]") {
  const EvaValue b(true);
  REQUIRE(isBoolean(b) == true);
  REQUIRE(asBoolean(b) == true);
}

TEST_CASE("Boolean accessor", "[EvaValue]") {
  const EvaValue b(false);
  REQUIRE(asBoolean(b) == false);
}

TEST_CASE("Boolean test", "[EvaValue]") {
  const EvaValue b(true);
  const EvaValue s("foobar");
  REQUIRE(isBoolean(b) == true);
  REQUIRE(isBoolean(s) == false);
}
