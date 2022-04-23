#include "btree.h"

#include <iostream>
#include <string>


using namespace std;

enum TestResultState { PASS, FAIL };

struct TestResult {
  TestResultState state;
  string reason;

  TestResult(TestResultState _state, string _reason): state(_state), reason(_reason) {}

  static TestResult pass() {
    return TestResult(PASS, string());
  }

  static TestResult fail(string reason) {
    return TestResult(FAIL, reason);
  }

  void print(string name) {
    if(state == PASS)
      cout << "-----" << name << "----- PASSED" << endl;
    else {
      cout << "-----" << name << "----- FAILED" << endl;
      cout << reason << endl;
      cout << "-----------------" << endl;
    }
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

TestResult test_btree_insert(int t, std::vector<int> input_stream, std::string expected) {
  BTree btree = BTree(t);
  for(auto i: input_stream)
    btree.insert(i);
  std::string btree_repr = btree.to_string();
  if(btree_repr == expected)
    return TestResult::pass();
  else
    return TestResult::fail("BTree(" + btree_repr + ") != Expected(" + expected + ")");
}

TestResult test_split_root() {
  // Start with a non-full node with a full node as a child
  // call split_child
  BTree btree = BTree(2);
  btree.insert(1);
  btree.insert(3);
  btree.insert(5);
  btree.insert(4);
  // btree.insert(7);
  // btree.insert(8);
  // btree.insert(9);
  // btree.insert(2);
  // btree.insert(6);
  // btree.insert(10);
  // btree.insert(12);


  if(btree.root->isLeaf == true)
    return TestResult::fail("Root node can't be leaf node after split");
  if(!((1 <= btree.root->n) && (btree.root->n <= (2*btree.t - 1))))
    return TestResult::fail("(1 <= btree.root.n <= 2*t -1) is false");

  if(!(btree.root->n == 1))
    return TestResult::fail("Root node size is wrong");
  if(!(btree.root->keys.size() == 1))
    return TestResult::fail("Root node keys size is wrong");
  if(!(btree.root->children.size() == 2))
    return TestResult::fail("Root node children size is wrong");
  if(!(btree.root->keys == vector<int>{3}))
    return TestResult::fail("Root node elements are wrong: " + string(btree.root->keys.begin(), btree.root->keys.end()));

  auto child_0 = btree.root->children[0];
  auto child_1 = btree.root->children[1];

  if(!(child_0->n == 1))
    return TestResult::fail("child_0 n is wrong");
  if(!(child_0->keys.size() == 1))
    return TestResult::fail("child_0 keys size is wrong");
  if(!(child_0->children.size() == 0))
    return TestResult::fail("child_0 children size is wrong");
  if(!(child_0->keys == vector<int>{1}))
    return TestResult::fail("child_0 key elements are wrong: ");


  if(!(child_1->n == 2))
    return TestResult::fail("child_1 n is wrong");
  if(!(child_1->keys.size() == 2))
    return TestResult::fail("child_1 keys size is wrong");
  if(!(child_1->children.size() == 0))
    return TestResult::fail("child_1 children size is wrong");
  if(!(child_1->keys == vector<int>{4,5}))
    return TestResult::fail("child_1 key elements are wrong");

  return TestResult::pass();
}


/** Test Order:
 * 1. split_child
 * 2. split_root
 * 3. _insert
 * 4. insert
 *
*/

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

// TestResult test_insert_adds_element_in_leaf_node() {
//   int input_set[] = {7, 4};
//   int t = 2;

//   // Create a BTree
//   BTree tree = BTree(t);

//   // Insert node
//   for(auto i : input_set)
//     tree.insert(i);

//   // Traverse BTree
//   std::shared_ptr<BTreeNode> root = tree.root;
//   if(root->n != 3)
//     return TestResult::fail("B-Tree root node is not of expected size");
//   if(root->keys != vector<int>{7, 4, 3})
//     return TestResult::fail("B-Tree root node keys incorrect");
//   if(root->children.size() != 0)
//     return TestResult::fail("B-Tree child pointers incorrect");

//   return TestResult::pass();
// }

int main(int argc, char** argv) {
  test_node_construction().print("Node construction");
  test_btree_insert(2, std::vector<int>{1,3,5,4,7,8,9,2,6,10,12}, "BTree([keys={5},children={[keys={3},children={[keys={1,2},children={}],[keys={4},children={}]}],[keys={8},children={[keys={6,7},children={}],[keys={9,10,12},children={}]}]}])").print("BTree Construction");

  //test_insert_adds_element_in_leaf_node().print("B-Tree insert");
}
/**
 *  Test Cases:
 * Constructor
 * 1. Node is created with the correct attributes
 * Search
 * 1. Returns the correct BTreeSearchResult when element present
 * 2. Returns empty BTreeSearchResult when element is not present
 * Insert
 * 1. Inserts element in the leaf node
 *  Simple head insert is a leaf node insert
 * 2. Splits full non-leaf node to insert element
 *  Construct a big enough tree from an input with a low `t`, and there will be a non-leaf node split.
 * 3. Splits leaf full node to insert element
 *  When constructing a small tree, it should split a leaf node to increase the height of the tree.
 * 4. Splits full root node to insert element
 *  It will do this to increase the height of the tree.
*/
