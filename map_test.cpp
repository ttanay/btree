#include <stdexcept>
#include "map.h"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

TEST_CASE("btree::Map is initialized", "[btree::Map]")
{
  btree::Map<int, int> m = {{1, 10}, {2, 20}, {3, 30}};
  REQUIRE(m[1].value == 10);
  REQUIRE(m[2].value == 20);
  REQUIRE(m[3].value == 30);
}

TEST_CASE("btree::Map at works", "[btree::Map]")
{
  btree::Map<int, int> m = {{1, 10}, {2, 20}, {3, 30}};
  REQUIRE(m[1].value == 10);
  REQUIRE_THROWS_AS(m.at(100), std::out_of_range);
}

TEST_CASE("btree::Map clear works", "[btree::Map]")
{
  btree::Map<int, int> m = {{1, 10}, {2, 20}, {3, 30}};
  m.clear();
  REQUIRE_THROWS_AS(m.at(1), std::out_of_range);
  REQUIRE_THROWS_AS(m.at(2), std::out_of_range);
  REQUIRE_THROWS_AS(m.at(3), std::out_of_range);
}

TEST_CASE("btree::Map erase works", "[btree::Map]")
{
  btree::Map<int, int> m = {{1, 10}, {2, 20}, {3, 30}};
  m.erase(1);
  REQUIRE_THROWS_AS(m.at(1), std::out_of_range);
  REQUIRE(m.at(2) == 20);
}
