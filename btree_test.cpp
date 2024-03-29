#include "btree.h"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

TEST_CASE("BTree::insert works", "[BTree::insert]")
{
  btree::BTree btree{2, std::vector<int>{1, 3, 5, 4, 7, 8, 9, 2, 6, 10, 12}};
  std::string expected_repr = "BTree([keys={5},children={[keys={3},children={[keys={1,2},children={}],[keys={4},children={}]}],[keys={8},"
                              "children={[keys={6,7},children={}],[keys={9,10,12},children={}]}]}])";
  REQUIRE(btree.toString() == expected_repr);
}

TEST_CASE("BTree::insert throws on duplicate input", "[BTree::insert]")
{
  btree::BTree btree{2, std ::vector<int>{1, 3, 5, 4}};
  REQUIRE_THROWS_WITH(btree.insert(1), "Element already exists");
}

TEST_CASE("BTree::search works", "[BTree::search]")
{
  btree::BTree btree{2, std ::vector<int>{1, 3, 5, 4}};
  auto result = btree.search(5);
  REQUIRE(result.height == 1);
  REQUIRE(result.index == 1);
}

TEST_CASE("BTree::search not found", "[BTree::search]")
{
  btree::BTree btree{2, std::vector<int>{1}};
  auto result = btree.search(10);
  REQUIRE(result.isEmpty());
}

TEST_CASE("BTree::search empty tree", "[BTree::search]")
{
  auto result = btree::BTree<int>(2).search(100);
  REQUIRE(result.isEmpty());
}

TEST_CASE("BTree::min works", "[BTree::min]")
{
  btree::BTree btree{2, std::vector<int>{1, 3, 5, 4}};
  REQUIRE(btree.min() == 1);
}

TEST_CASE("BTree::min empty tree", "[BTree::min]")
{
  REQUIRE_THROWS_AS(btree::BTree<int>(2).min(), std::length_error);
}

TEST_CASE("BTree::max", "[BTree::max]")
{
  btree::BTree btree{2, std ::vector<int>{1, 3, 5, 4}};
  REQUIRE(btree.max() == 5);
}

TEST_CASE("BTree::max empty tree", "[BTree::max]")
{
  REQUIRE_THROWS_AS(btree::BTree<int>(2).max(), std::length_error);
}

TEST_CASE("BTree::del absent element", "[BTree::del]")
{
  btree::BTree btree{2, std ::vector<int>{1, 3, 5, 4}};
  btree.del(100);
  auto expected = "BTree([keys={3},children={[keys={1},children={}],[keys={4,5},children={}]}])";
  REQUIRE(btree.toString() == expected);
}

TEST_CASE("BTree::del leaf", "[BTree::del]")
{
  btree::BTree btree{2, std::vector<int>{1, 3, 5, 4, 7, 8, 9, 2, 6, 10, 12}};
  btree.del(3);
  auto expected = "BTree([keys={2,5,8},children={[keys={1},children={}],[keys={4},children={}],[keys={6,7},children={}],[keys={9,10,12},"
                  "children={}]}])";
  REQUIRE(btree.toString() == expected);
}

TEST_CASE("BTree::del root", "[BTree::del]")
{
  btree::BTree btree{2, std::vector<int>{1, 3, 5, 4, 7, 8, 9, 2, 6, 10, 12}};
  btree.del(5);
  auto expected = "BTree([keys={3,6,8},children={[keys={1,2},children={}],[keys={4},children={}],[keys={7},children={}],[keys={9,10,12},"
                  "children={}]}])";
  REQUIRE(btree.toString() == expected);
}

TEST_CASE("BTree::del merge children", "[BTree::del]")
{
  btree::BTree btree{2, std::vector<int>{1, 3, 5, 4, 7, 8, 9, 2, 6, 10, 12}};
  btree.del(3);
  btree.del(2);
  auto expected= "BTree([keys={5,8},children={[keys={1,4},children={}],[keys={6,7},children={}],[keys={9,10,12},children={}]}])";
  REQUIRE(btree.toString() == expected);
}

TEST_CASE("BTree::del right most leaf element", "[BTree::del]")
{
  btree::BTree btree{2, std::vector<int>{4, 5, 6, 2}};
  btree.del(6);
  auto expected = "BTree([keys={4},children={[keys={2},children={}],[keys={5},children={}]}])";
  REQUIRE(btree.toString() == expected);
}

TEST_CASE("BTree::del left most leaf element", "[BTree::del]")
{
  btree::BTree btree{2, std::vector<int>{1, 5, 4, 6}};
  btree.del(1);
  auto expected = "BTree([keys={5},children={[keys={4},children={}],[keys={6},children={}]}])";
  REQUIRE(btree.toString() == expected);
}

TEST_CASE("Btree:del empty tree", "[BTree::del]")
{
  auto btree = btree::BTree<int>(2);
  btree.del(5);
  REQUIRE(btree.toString() == "BTree()");
}

TEST_CASE("BTree::clear works", "[BTree::clear]")
{
  btree::BTree btree{2, std::vector<int>{4, 5, 6, 2}};
  btree.clear();
  auto expected = "BTree()";
  REQUIRE(btree.toString() == expected);
}
