#include <iostream>
#include <memory>
#include <sstream>
#include <vector>


template <typename T>
struct BTreeNode
{
  // no of keys stored in the node
  int n;
  // list of `n` keys
  std::vector<T> keys;
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

template <typename T>
using BTreeNodePtr = std::shared_ptr<BTreeNode<T>>;

template <typename T>
struct BTreeSearchResult
{
  BTreeNodePtr<T> node;
  int height;
  int index;

  bool isEmpty() { return (node == nullptr); }
};

template <typename T>
class BTree
{
  // The minimum_degree
  int t;
  // The root of the tree
  BTreeNodePtr<T> root;

  bool isFull(BTreeNodePtr<T> node);
  T min(BTreeNodePtr<T> node);
  T max(BTreeNodePtr<T> node);
  BTreeSearchResult<T> search(BTreeNodePtr<T> node, T element, int height);
  std::string toString(BTreeNodePtr<T> node);

  void insert(BTreeNodePtr<T> node, T element);
  void splitChild(BTreeNodePtr<T> node, int index);
  void splitRoot();

  void del(BTreeNodePtr<T> node, T element);
  void mergeChildren(BTreeNodePtr<T> node, int index);
  int appropriateIndex(BTreeNodePtr<T> node, T element);
  void delInternalNode(BTreeNodePtr<T> node, int i, T element);
  void ensureChildCanAccomodate(BTreeNodePtr<T> node, int index);

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
  BTree(int minimum_degree, std::vector<T> seed_stream) : BTree<T>(minimum_degree)
  {
    for (T i : seed_stream)
      insert(i);
  }

  /**
   * Search for an element in the BTree
   * @param element The element to search for
   * @return the search result
   */
  BTreeSearchResult<T> search(T element);

  /**
   * Find the minimum element in the BTree
   * @return the min element
   */
  T min();

  /**
   * Find the maximum element in the BTree
   * @return the max element
   */
  T max();

  /**
   * Insert an element into the BTree
   * @param element to be inserted
   */
  void insert(T element);

  /**
   * Delete element from BTree if it exists
   * @param element to be deleted
   */
  void del(T element);

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

template <typename T>
bool BTree<T>::isFull(BTreeNodePtr<T> node)
{
  return node->n == 2 * t - 1;
}

template <typename T>
T BTree<T>::min()
{
  if (!root)
    throw std::length_error("BTree is empty");

  return min(root);
}

template <typename T>
T BTree<T>::min(BTreeNodePtr<T> node)
{
  if (node->is_leaf)
    return node->keys[0];
  else
    return min(node->children[0]);
}

template <typename T>
T BTree<T>::max()
{
  if (!root)
    throw std::length_error("BTree is empty");

  return max(root);
}

template <typename T>
T BTree<T>::max(BTreeNodePtr<T> node)
{
  auto n = node->n;
  if (node->is_leaf)
    return node->keys[n - 1];
  else
    return max(node->children[n]);
}

template <typename T>
BTreeSearchResult<T> BTree<T>::search(T element)
{
  if (!root)
    return BTreeSearchResult<T>{};

  return search(root, element, 0);
}

template <typename T>
BTreeSearchResult<T> BTree<T>::search(BTreeNodePtr<T> node, T element, int height)
{
  int i = 0;
  // Find appropriate index where the key is likely to be found
  while (i < node->n && node->keys[i] < element)
    i++;

  if (i < node->n && node->keys[i] == element)
    // TODO: Type deduction doesn't work here for some reason. Investigate
    return BTreeSearchResult<T>{node, height, i};
  else if (node->is_leaf)
    return BTreeSearchResult<T>{};
  else
    return search(node->children[i], element, height + 1);
}

template <typename T>
void BTree<T>::insert(T element)
{
  if (!search(element).isEmpty())
    throw std::invalid_argument("Element already exists: " + std::to_string(element));

  if (!root)
    root = std::make_unique<BTreeNode<T>>();

  if (isFull(root))
    splitRoot();

  // insert non-root
  insert(root, element);
}

template <typename T>
void BTree<T>::splitRoot()
{
  BTreeNodePtr<T> new_root = std::make_shared<BTreeNode<T>>();
  new_root->is_leaf = false;
  new_root->children.insert(new_root->children.begin(), root);
  root = new_root;
  splitChild(root, 0);
}

template <typename T>
void BTree<T>::insert(BTreeNodePtr<T> node, T element)
{
  if (node->is_leaf)
  {
    // The assumption here is that the leaf node will always be non-full
    //  => n < 2t -1
    int i = node->n - 1;
    // Move keys one place to the right to make space to insert the element
    for (; i >= 0 && node->keys[i] > element; i--)
      ;

    // Insert at the correct index
    node->keys.insert(node->keys.begin() + i + 1, element);
    // Increment size
    node->n += 1;
  }
  else
  {
    // Find the appropriate location
    int i = node->n - 1;
    for (; i >= 0 && node->keys[i] > element; i--)
      ;
    // The appropriate place will be the right child of the key
    i += 1;

    // Split child if node is full
    if (node->children[i]->n == (2 * t - 1))
    {
      splitChild(node, i);
      // Increment index in case the key propogated up from the split
      // is larger than the previous index
      if (node->keys[i] < element)
        i += 1;
    }

    // Insert in child node
    insert(node->children[i], element);
  }
}

template <typename T>
void BTree<T>::splitChild(BTreeNodePtr<T> node, int index)
{
  BTreeNodePtr<T> new_child = std::make_shared<BTreeNode<T>>();
  BTreeNodePtr<T> child = node->children[index];

  new_child->is_leaf = child->is_leaf;
  new_child->n = t - 1;

  // Copy over keys at indices [t-1, 2t-1) to new node
  new_child->keys.insert(new_child->keys.begin(), child->keys.begin() + t, child->keys.end());

  // Remove excess keys from the node
  child->keys.resize(t - 1);

  if (!child->is_leaf)
  {
    // Copy over child nodes from indices [t, 2t)
    new_child->children.insert(new_child->children.begin(), child->children.begin() + t, child->children.end());

    // Remove the excess child pointers from the array
    child->children.resize(t);
  }

  // Set size of child to t-1
  child->n = t - 1;

  // Move node's children pointers right one index
  // The last index for children is n+1
  // We want to move elements in (index, n+1] to the right
  for (int i = node->n; i > index + 1; i--)
    node->children.insert(node->children.begin() + i + 1, node->children[i]);

  // Make the new child node the child of the parent
  node->children.emplace(node->children.begin() + index + 1, new_child);

  // Move node's keys to the right by one index
  for (int i = node->n - 1; i >= index; i--)
    node->keys.insert(node->keys.begin() + i + 1, node->keys[i]);

  // move median key from child up into parent
  node->keys.emplace(node->keys.begin() + index, child->keys[t - 1]);

  // Increment size of keys in `node`
  node->n += 1;
}

template <typename T>
std::string BTree<T>::toString()
{
  std::ostringstream btree_repr;
  btree_repr << "BTree(" << toString(root) << ")";
  return btree_repr.str();
}

template <typename T>
std::string BTree<T>::toString(BTreeNodePtr<T> node)
{
  if (node == nullptr)
    return "";

  std::ostringstream node_repr;
  node_repr << "[keys={";
  for (auto i : node->keys)
  {
    node_repr << i;
    if (!(i == node->keys.back()))
      node_repr << ",";
  }
  node_repr << "},children={";
  for (auto c : node->children)
  {
    node_repr << toString(c);
    if (!(c == node->children.back()))
      node_repr << ",";
  }
  node_repr << "}]";
  return node_repr.str();
}

template <typename T>
void BTree<T>::del(T element)
{
  // We ascertain here that the tree contains the element
  // so that we don't modify the tree unnecessarily
  if (search(element).isEmpty())
    return;

  del(root, element);
}

template <typename T>
void BTree<T>::del(BTreeNodePtr<T> node, T element)
{
  int i = appropriateIndex(node, element);
  // If the element to delete is contained in the node
  if (node->keys[i] == element)
  {
    if (node->is_leaf)
    {
      // The node is guaranteed to have at least t keys
      // We can simply delete the key
      node->keys.erase(node->keys.begin() + i);
      node->n--;
    }
    else
      delInternalNode(node, i, element);
  }
  else
  {
    if (node->children[i]->n < t)
      ensureChildCanAccomodate(node, i);
    del(node->children[i], element);
  }
}

template <typename T>
int BTree<T>::appropriateIndex(BTreeNodePtr<T> node, T element)
{
  int i = 0;
  for (; i < node->n && node->keys[i] < element; i++)
    ;
  return i;
}

template <typename T>
void BTree<T>::delInternalNode(BTreeNodePtr<T> node, int i, T element)
{
  // If left child has >= t keys
  // Delete predecessor from left child's subtree
  // Replace the element with the predecessor
  auto left_child = node->children[i];
  if (left_child->n >= t)
  {
    auto replacement = max(left_child);
    del(left_child, replacement);
    node->keys[i] = replacement;
  }
  else
  {
    // If the right child has >= t keys
    // Delete successor from right child's subtree
    // Replace the element with the successor
    auto right_child = node->children[i + 1];
    if (right_child->n >= t)
    {
      auto replacement = min(right_child);
      del(right_child, replacement);
      node->keys[i] = replacement;
    }
    else
    {
      // Merge children and push the element to the merged node
      mergeChildren(node, i);
      // Delete the element from the merged node
      del(left_child, element);
    }
  }
}

template <typename T>
void BTree<T>::mergeChildren(BTreeNodePtr<T> node, int index)
{
  auto left_child = node->children[index];
  auto right_child = node->children[index + 1];
  left_child->n += right_child->n + 1;

  // Move element to left_child
  left_child->keys.push_back(node->keys[index]);

  // Copy keys from right_child to left_child
  left_child->keys.insert(left_child->keys.end(), right_child->keys.begin(), right_child->keys.end());

  // Copy children pointers from right_child to left_child
  left_child->children.insert(left_child->children.end(), right_child->children.begin(), right_child->children.end());

  // Delete key and pointer from node
  node->keys.erase(node->keys.begin() + index);
  node->children.erase(node->children.begin() + index + 1);
  node->n -= 1;

  // Update root to the the merged child if node is root
  if (root == node && node->n == 0)
    root = left_child;
}

template <typename T>
void BTree<T>::ensureChildCanAccomodate(BTreeNodePtr<T> node, int index)
{
  // Check if left sibling can donate
  auto to_recurse = node->children[index];
  if (index > 0 && node->children[index - 1]->n >= t)
  {
    auto left_sibling = node->children[index - 1];
    // Move key from node to child
    to_recurse->keys.insert(to_recurse->keys.begin(), node->keys[index - 1]);
    // Move key from sibling to node
    node->keys[index - 1] = left_sibling->keys.back();
    left_sibling->keys.pop_back();
    // Move child ptr from sibling to child
    if (!to_recurse->is_leaf)
    {
      to_recurse->children.insert(to_recurse->children.begin(), left_sibling->children.back());
      left_sibling->children.pop_back();
    }
  }
  // Check if right sibling can donate
  else if (index < node->n && node->children[index + 1]->n >= t)
  {
    auto right_sibling = node->children[index + 1];
    // Move key from node to child
    to_recurse->keys.push_back(node->keys[index]);
    // Move key from sibling to node
    node->keys[index] = right_sibling->keys.front();
    right_sibling->keys.erase(right_sibling->keys.begin());
    // Move child ptr from sibling to child
    if (!to_recurse->is_leaf)
    {
      to_recurse->children.push_back(right_sibling->children.front());
      right_sibling->children.erase(right_sibling->children.begin());
    }
  }
  // Merge with sibling
  else
  {
    if (index == node->n)
      // Merge with left child
      mergeChildren(node, index - 1);
    else
      mergeChildren(node, index);
  }
}
