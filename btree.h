#include <iostream>
#include <vector>
#include <memory>

struct BTreeNode {
  int n; // no of keys stored in the node
  std::vector<int> keys; // list of `n` keys
  bool isLeaf; // whether the node is a leaf
  std::vector<std::shared_ptr<BTreeNode>> children; // Array of the BTree Node's children

  //default constructor
  BTreeNode() {
    n = 0;
    isLeaf = true;
  }
};

using BTreeNodePtr = std::shared_ptr<BTreeNode>;

struct BTreeSearchResult {
  BTreeNode node;
  int index;
};

class BTree {
  public:

  int t; // The minimum_degree

  // Check whether the node is full(`n = 2*t - 1`)
  bool is_full(BTreeNodePtr node);
  BTreeSearchResult _search(BTreeNodePtr node, int element);
  void split_child(BTreeNodePtr node, int index);
  void split_root();
  void _insert(BTreeNodePtr node, int element);
  std::string _to_string(BTreeNodePtr node);

  // TODO: Make this private. This is public for test.
  BTreeNodePtr root;

  BTree(int minimum_degree) {
    if(minimum_degree < 2)
      throw std::invalid_argument("minimum_degree should be >= 2");
    t = minimum_degree;
    root = std::make_unique<BTreeNode>();
  }

  BTreeSearchResult search(int element);
  void insert(int element);

  /** String representation of a BTree is like the follows:
   *   BTree([{3, 5, 8}, [[{1, 2}], [{4}], [{6, 7}], [{10}, [{9}], [{12, 14}]]]])
   *
   *   Here, each array is a node.
   *   The first element enclosed in `{}` in the array are the keys.
   *   The second element enclosed in `[]` are the child nodes in order.
   *   The child nodes are in the same format.
   */
  std::string to_string();
};
