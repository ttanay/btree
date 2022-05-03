#include <iostream>
#include <vector>
#include <memory>

struct BTreeNode
{
  // no of keys stored in the node
  int n;
  // list of `n` keys
  std::vector<int> keys;
  // whether the node is a leaf
  bool isLeaf;
  // Array of the BTree Node's children
  std::vector<std::shared_ptr<BTreeNode>> children;

  BTreeNode()
  {
    n = 0;
    isLeaf = true;
  }
};

using BTreeNodePtr = std::shared_ptr<BTreeNode>;

struct BTreeSearchResult
{
  BTreeNodePtr node;
  int height;
  int index;

  bool empty()
  {
    return (!height && !index);
  }
};

class BTree
{
  // The minimum_degree
  int t;
  // The root of the tree
  BTreeNodePtr root;

  // Check whether the node is full(`n = 2*t - 1`)
  bool is_full(BTreeNodePtr node);
  BTreeSearchResult _search(BTreeNodePtr node, int element, int height);
  void split_child(BTreeNodePtr node, int index);
  void split_root();
  void _insert(BTreeNodePtr node, int element);
  std::string _to_string(BTreeNodePtr node);

public:
  BTree(int minimum_degree)
  {
    if (minimum_degree < 2)
      throw std::invalid_argument("minimum_degree should be >= 2");
    t = minimum_degree;
    root = std::make_unique<BTreeNode>();
  }

  BTreeSearchResult search(int element);
  void insert(int element);

  /** String representation of a BTree is like the follows:
   *   BTree(keys={5},children={[keys={3},children={}],[keys={8},children={}]})
   *   Here, each array is a node.
   *   The child nodes are in the same format.
   */
  std::string to_string();
};
