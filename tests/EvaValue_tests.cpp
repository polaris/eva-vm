#include <catch2/catch_test_macros.hpp>

#include "../src/vm/EvaValue.h"

TEST_CASE("Number construction", "[EvaValue]") {
  const EvaValue n = Number(1.0);
  REQUIRE(isNumber(n) == true);
  REQUIRE(asNumber(n) == 1.0);
}

TEST_CASE("Number accessor", "[EvaValue]") {
  const EvaValue n = Number(1.0);
  REQUIRE(asNumber(n) == 1.0);
}

TEST_CASE("Number test", "[EvaValue]") {
  const EvaValue n = Number(1.0);
  const EvaValue s = allocString("foobar");
  REQUIRE(isNumber(n) == true);
  REQUIRE(isNumber(s) == false);
  deallocString(s);
}

TEST_CASE("String construction", "[EvaValue]") {
  const EvaValue s = allocString("foobar");
  REQUIRE(isString(s) == true);
  REQUIRE(asCppString(s) == "foobar");
  deallocString(s);
}

TEST_CASE("String accessor", "[EvaValue]") {
  const EvaValue s = allocString("Hello, world!");
  REQUIRE(asString(s)->type == ObjectType::String);
  deallocString(s);
}

TEST_CASE("C++ string accessor", "[EvaValue]") {
  const EvaValue s = allocString("Hello, world!");
  REQUIRE(asCppString(s) == "Hello, world!");
  deallocString(s);
}

TEST_CASE("String test", "[EvaValue]") {
  const EvaValue s = allocString("foobar");
  const EvaValue n = Number(1.0);
  REQUIRE(isString(s) == true);
  REQUIRE(isString(n) == false);
  deallocString(s);
}

TEST_CASE("Object accessor", "[EvaValue]") {
  const EvaValue s = allocString("Hello, world!");
  REQUIRE(asObject(s)->type == ObjectType::String);
  deallocString(s);
}

TEST_CASE("Object type test", "[EvaValue]") {
  const EvaValue s = allocString("foobar");
  REQUIRE(isObjectType(s, ObjectType::String) == true);
  deallocString(s);
}

TEST_CASE("Object test", "[EvaValue]") {
  const EvaValue s = allocString("foobar");
  const EvaValue n = Number(1.0);
  REQUIRE(isObject(s) == true);
  REQUIRE(isObject(n) == false);
  deallocString(s);
}

TEST_CASE("Boolean construction", "[EvaValue]") {
  const EvaValue b = Boolean(1.0);
  REQUIRE(isBoolean(b) == true);
  REQUIRE(asBoolean(b) == 1.0);
}

TEST_CASE("Boolean accessor", "[EvaValue]") {
  const EvaValue b = Boolean(1.0);
  REQUIRE(asBoolean(b) == 1.0);
}

TEST_CASE("Boolean test", "[EvaValue]") {
  const EvaValue b = Boolean(1.0);
  const EvaValue s = allocString("foobar");
  REQUIRE(isBoolean(b) == true);
  REQUIRE(isBoolean(s) == false);
  deallocString(s);
}
