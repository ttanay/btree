#include <iostream>
#include <string>
#include "btree.h"

using namespace std;

enum TestResultState { PASS, FAIL };

struct TestResult {
  TestResultState state;
  string reason;

  TestResult(TestResultState state, string reason) {
    this->state = state;
    this->reason = reason;
  }

  static TestResult pass() {
    return TestResult(PASS, string());
  }

  static TestResult fail(string reason) {
    return TestResult(FAIL, reason);
  }

  void print(string name) {
    if(state == PASS)
      cout << "-----" << name << "----- PASSED" << endl;
    else
      cout << "-----" << name << "----- FAILED" << endl;
  }
};

TestResult test_node_construction() {
  BTreeNode node = BTreeNode();
  if(node.n != 0)
    return TestResult::fail("New node size is not 0");
  if(node.isLeaf != true)
    return TestResult::fail("New node is not a leaf");
  if(node.keys.size() != 0)
    return TestResult::fail("New node doesn't have empty keys");
  if(node.children.size() != 0)
    return TestResult::fail("New node doesn't have child pointers empty");

  return TestResult::pass();
}

// TestResult test_search_correct_when_element_present() {
// We test this with a tree of `t = 2`
// [6, 13, 20] ---> [22, 23, 25]
//             |--> [15, 17, 18]
//             |--> [8, 10, 11]
//             |--> [1, 3, 5]
// BTreeNode root = {3, vector<int>{6, 13, 20}, vector<BTreeNode*>};
// BTreeNode child1 = {3, vector<int>{1, 3, 5}, vector<BTreeNode*>};
// BTreeNode child2 = {3, vector<int>{8, 7, 10}, vector<BTreeNode*>};
// BTreeNode child3 = {3, vector<int>{15, 17, 10}, vector<BTreeNode*>};
// BTreeNode child4 = {3, vector<int>{22, 23, 25}, vector<BTreeNode*>};
// }

TestResult test_insert_adds_element_in_leaf_node() {
  int input_set[] = {7, 4};
  int t = 2;

  // Create a BTree
  BTree tree = BTree(t);

  // Insert node
  for(auto i : input_set)
    tree.insert(i);

  // Traverse BTree
  BTreeNode *root = tree.root;
  if(root->n != 3)
    return TestResult::fail("B-Tree root node is not of expected size");
  if(root->keys != vector<int>{7, 4, 3})
    return TestResult::fail("B-Tree root node keys incorrect");
  if(root->children.size() != 0)
    return TestResult::fail("B-Tree child pointers incorrect");

  return TestResult::pass();
}

int main(int argc, char** argv) {
  test_node_construction().print("Node construction");

  test_insert_adds_element_in_leaf_node().print("B-Tree insert");
}
// Test Cases:
//
// Constructor
// 1. Node is created with the correct attributes
//
// Search
// 1. Returns the correct BTreeSearchResult when element present
// 2. Returns empty BTreeSearchResult when element is not present
//
// Insert
// 1. Inserts element in the leaf node
// 2. Splits full node to insert element
// 3. Splits full root node to insert element
