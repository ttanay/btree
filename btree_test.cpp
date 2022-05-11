#include "btree.h"

#include <iostream>
#include <functional>
#include <string>

using namespace std;

enum class TestResultState
{
  PASS,
  FAIL
};

// TODO: Make it a separate header file
struct TestResult
{
  TestResultState state;
  string reason;

  TestResult(TestResultState _state, string _reason) : state(_state), reason(_reason) {}

  static TestResult pass()
  {
    return TestResult(TestResultState::PASS, string());
  }

  static TestResult fail(string reason)
  {
    return TestResult(TestResultState::FAIL, reason);
  }

  void print(string name)
  {
    if (state == TestResultState::PASS)
      cout << "PASSED:-----" << name << "-----" << endl;
    else
    {
      cout << "FAILED:-----" << name << "-----" << endl;
      cerr << reason << endl;
      cout << "-----------------" << endl;
    }
  }
};

TestResult test_btree_insert(int t, std::vector<int> input_stream, std::string expected)
{
  BTree btree = BTree(t);
  for (auto i : input_stream)
    btree.insert(i);
  std::string btree_repr = btree.toString();
  if (btree_repr == expected)
    return TestResult::pass();
  else
    return TestResult::fail("BTree(" + btree_repr + ") != Expected(" + expected + ")");
}

TestResult test_btree_search(BTree &btree, int element, int expected_height, int expected_index)
{
  auto result = btree.search(element);
  if (result.isEmpty())
    return TestResult::fail("Result is empty");
  if (result.height != expected_height)
    return TestResult::fail("height != expected_height");
  if (result.index != expected_index)
    return TestResult::fail("index != expected_index");

  return TestResult::pass();
}

TestResult test_btree_search_not_found(BTree btree, int element)
{
  auto result = btree.search(element);
  if (!result.isEmpty())
    return TestResult::fail("Result is not empty");

  return TestResult::pass();
}

TestResult test_btree_min(BTree btree, int expected)
{
  auto result = btree.min();
  if (result == expected)
    return TestResult::pass();
  else
    return TestResult::fail(to_string(result) + " != " + to_string(expected));
}

TestResult test_btree_length_error(std::function<void()> test_fn)
{
  try
  {
    test_fn();
  }
  catch (const std::length_error &e)
  {
    return TestResult::pass();
  }
  catch (const std::exception &e)
  {
    return TestResult::fail("Expected std::length_error; Got: " + string(e.what()));
  }
  return TestResult::fail("No exception raised");
}

TestResult test_btree_invalid_argument_error(std::function<void()> test_fn)
{
  try
  {
    test_fn();
  }
  catch (const std::invalid_argument &e)
  {
    return TestResult::pass();
  }
  catch (const std::exception &e)
  {
    return TestResult::fail("Expected std::invalid_argument; Got: " + string(e.what()));
  }
  return TestResult::fail("No exception raised");
}

TestResult test_btree_max(BTree btree, int expected)
{
  auto result = btree.max();
  if (result == expected)
    return TestResult::pass();
  else
    return TestResult::fail(to_string(result) + " != " + to_string(expected));
}

TestResult test_btree_del(BTree btree, int element, std::string expected_tree)
{
  btree.del(element);
  auto btree_repr = btree.toString();
  if (btree_repr == expected_tree)
    return TestResult::pass();
  else
    return TestResult::fail(btree_repr + " != " + expected_tree);
}

int main(int argc, char **argv)
{
  test_btree_insert(2, std::vector<int>{1, 3, 5, 4, 7, 8, 9, 2, 6, 10, 12}, "BTree([keys={5},children={[keys={3},children={[keys={1,2},children={}],[keys={4},children={}]}],[keys={8},children={[keys={6,7},children={}],[keys={9,10,12},children={}]}]}])").print("BTree::insert");

  BTree btree = BTree(2, std ::vector<int>{1, 3, 5, 4});
  test_btree_invalid_argument_error([&btree]
                                    { btree.insert(1); })
      .print("BTree::insert duplicate input");

  test_btree_search(btree, 5, 1, 1)
      .print("BTree::search");
  test_btree_search_not_found(btree, 100).print("BTree::search not found");
  test_btree_search_not_found(BTree(2), 1).print("BTree::search empty tree");
  test_btree_min(btree, 1).print("BTree::min");
  test_btree_length_error([]
                          { BTree(2).min(); })
      .print("BTree::min empty error raised");
  test_btree_max(btree, 5)
      .print("BTree::max");
  test_btree_length_error([]
                          { BTree(2).max(); })
      .print("BTree::max empty error raised");

  test_btree_del(btree, 100, "BTree([keys={3},children={[keys={1},children={}],[keys={4,5},children={}]}])").print("BTree::del absent element");
  test_btree_del(BTree(2, std::vector<int>{1, 3, 5, 4, 7, 8, 9, 2, 6, 10, 12}), 5, "BTree([keys={3,6,8},children={[keys={1,2},children={}],[keys={4},children={}],[keys={7},children={}],[keys={9,10,12},children={}]}])").print("BTree::del root");
  btree = BTree(2, std::vector<int>{1, 3, 5, 4, 7, 8, 9, 2, 6, 10, 12});
  test_btree_del(btree, 3, "BTree([keys={2,5,8},children={[keys={1},children={}],[keys={4},children={}],[keys={6,7},children={}],[keys={9,10,12},children={}]}])").print("BTree::del leaf");
  test_btree_del(btree, 2, "BTree([keys={},children={[keys={5,8},children={[keys={1,4},children={}],[keys={6,7},children={}],[keys={9,10,12},children={}]}]}])").print("BTree::del merge children");
  test_btree_del(BTree(2, std::vector<int>{4, 5, 6, 2}), 6, "BTree([keys={4},children={[keys={2},children={}],[keys={5},children={}]}])").print("BTree::del right most leaf element");
  test_btree_del(BTree(2, std::vector<int>{1, 5, 4, 6}), 1, "BTree([keys={5},children={[keys={4},children={}],[keys={6},children={}]}])").print("BTree::del left most leaf element");
  test_btree_del(BTree(2), 4, "BTree()").print("BTree::del empty tree");
  // TODO: If failures are present return a non-zero exit code
  return 0;
}
