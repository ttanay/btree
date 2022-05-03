#include "btree.h"

#include <iostream>
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
      cout << "-----" << name << "----- PASSED" << endl;
    else
    {
      cout << "-----" << name << "----- FAILED" << endl;
      cout << reason << endl;
      cout << "-----------------" << endl;
    }
  }
};

TestResult test_btree_insert(int t, std::vector<int> input_stream, std::string expected)
{
  BTree btree = BTree(t);
  for (auto i : input_stream)
    btree.insert(i);
  std::string btree_repr = btree.to_string();
  if (btree_repr == expected)
    return TestResult::pass();
  else
    return TestResult::fail("BTree(" + btree_repr + ") != Expected(" + expected + ")");
}

TestResult test_btree_search(BTree &btree, int element, int expected_height, int expected_index)
{
  auto result = btree.search(element);
  if (result.empty())
    return TestResult::fail("Result is empty");
  if (result.height != expected_height)
    return TestResult::fail("height != expected_height");
  if (result.index != expected_index)
    return TestResult::fail("index != expected_index");

  return TestResult::pass();
}

TestResult test_bree_search_not_found(BTree &btree, int element)
{
  auto result = btree.search(element);
  if (!result.empty())
    return TestResult::fail("Result is not empty");

  return TestResult::pass();
}

int main(int argc, char **argv)
{
  test_btree_insert(2, std::vector<int>{1, 3, 5, 4, 7, 8, 9, 2, 6, 10, 12}, "BTree([keys={5},children={[keys={3},children={[keys={1,2},children={}],[keys={4},children={}]}],[keys={8},children={[keys={6,7},children={}],[keys={9,10,12},children={}]}]}])").print("BTree Insert");

  BTree btree = BTree(2);
  for (auto i : std::vector<int>{1, 3, 5, 4})
    btree.insert(i);

  test_btree_search(btree, 5, 1, 1).print("BTree Search");
  test_bree_search_not_found(btree, 100).print("BTree Search Not Found");

  return 0;
}
