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
  bool is_leaf;
  // Array of the BTree Node's children
  std::vector<std::shared_ptr<BTreeNode>> children;

  BTreeNode()
  {
    n = 0;
    is_leaf = true;
  }
};

using BTreeNodePtr = std::shared_ptr<BTreeNode>;

struct BTreeSearchResult
{
  BTreeNodePtr node;
  int height;
  int index;

  bool isEmpty()
  {
    return (node == nullptr);
  }
};

class BTree
{
  // The minimum_degree
  int t;
  // The root of the tree
  BTreeNodePtr root;

  bool isFull(BTreeNodePtr node);
  int min(BTreeNodePtr node);
  int max(BTreeNodePtr node);
  BTreeSearchResult search(BTreeNodePtr node, int element, int height);
  std::string toString(BTreeNodePtr node);

  void insert(BTreeNodePtr node, int element);
  void splitChild(BTreeNodePtr node, int index);
  void splitRoot();

  void del(BTreeNodePtr node, int element);
  void mergeChildren(BTreeNodePtr node, int index);
  int appropriateIndex(BTreeNodePtr node, int element);
  void delInternalNode(BTreeNodePtr node, int i, int element);
  void ensureChildCanAccomodate(BTreeNodePtr node, int element);

public:
  /**
   * Initialize a BTree object
   * @param minimum_degree The minimum no of child nodes a non-root node can have
   */
  BTree(int minimum_degree)
  {
    if (minimum_degree < 2)
      throw std::invalid_argument("minimum_degree should be >= 2");
    t = minimum_degree;
  }

  /**
   * Initialize a BTree object
   * @param minimum_degree The minimum no of child nodes a non-root node can have
   * @param seed_stream A vector of elements to initialize/seed the tree with
   */
  BTree(int minimum_degree, std::vector<int> seed_stream) : BTree(minimum_degree)
  {
    for (auto i : seed_stream)
      insert(i);
  }

  /**
   * Search for an element in the BTree
   * @param element The element to search for
   * @return the search result
   */
  BTreeSearchResult search(int element);

  /**
   * Find the minimum element in the BTree
   * @return the min element
   */
  int min();

  /**
   * Find the maximum element in the BTree
   * @return the max element
   */
  int max();

  /**
   * Insert an element into the BTree
   * @param element to be inserted
   */
  void insert(int element);

  /**
   * Delete element from BTree if it exists
   * @param element to be deleted
   */
  void del(int element);

  /** String representation of a BTree is like the follows:
   *   BTree(keys={5},children={[keys={3},children={}],[keys={8},children={}]})
   * Tree for it:
   *                    ------
   *                    | 5 |
   *                   ------
   *                 /       \
   *             ------     ------
   *             | 3 |      | 8 |
   *            ------     ------
   *   Here, each array is a node.
   *   The child nodes are in the same format.
   * @return the string representation
   */
  std::string toString();
};
