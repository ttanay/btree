#include "btree.h"

#include <sstream>

bool BTree::is_full(BTreeNodePtr node)
{
  return node->n == 2 * t - 1;
}

int BTree::min()
{
  if (!root)
    throw std::length_error("BTree is empty");

  return min(root);
}

int BTree::min(BTreeNodePtr node)
{
  if (node->isLeaf)
    return node->keys[0];
  else
    return min(node->children[0]);
}

int BTree::max()
{
  if (!root)
    throw std::length_error("BTree is empty");

  return max(root);
}

int BTree::max(BTreeNodePtr node)
{
  auto n = node->n;
  if (node->isLeaf)
    return node->keys[n - 1];
  else
    return max(node->children[n]);
}

BTreeSearchResult BTree::search(int element)
{
  if (!root)
    return BTreeSearchResult{};

  return search(root, element, 0);
}

BTreeSearchResult BTree::search(BTreeNodePtr node, int element, int height)
{
  int i = 0;
  // Find appropriate index where the key is likely to be found
  while (i < node->n && node->keys[i] < element)
    i++;

  if (node->keys[i] == element)
    return BTreeSearchResult{node, height, i};
  else if (node->isLeaf)
    return BTreeSearchResult{};
  else
    return search(node->children[i], element, height + 1);
}

void BTree::insert(int element)
{
  if (!search(element).empty())
    throw std::invalid_argument("Element already exists: " + element);

  if (!root)
    root = std::make_unique<BTreeNode>();

  if (is_full(root))
    split_root();

  // insert non-root
  insert(root, element);
}

void BTree::split_root()
{
  BTreeNodePtr new_root = std::make_shared<BTreeNode>();
  new_root->isLeaf = false;
  new_root->children.insert(new_root->children.begin(), root);
  root = new_root;
  split_child(root, 0);
}

void BTree::insert(BTreeNodePtr node, int element)
{
  if (node->isLeaf)
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
      split_child(node, i);
      // Increment index in case the key propogated up from the split
      // is larger than the previous index
      if (node->keys[i] < element)
        i += 1;
    }

    // Insert in child node
    insert(node->children[i], element);
  }
}

void BTree::split_child(BTreeNodePtr node, int index)
{
  BTreeNodePtr new_child = std::make_shared<BTreeNode>();
  BTreeNodePtr child = node->children[index];

  new_child->isLeaf = child->isLeaf;
  new_child->n = t - 1;

  // Copy over keys at indices [t-1, 2t-1) to new node
  new_child->keys.insert(new_child->keys.begin(), child->keys.begin() + t, child->keys.end());

  // Remove excess keys from the node
  child->keys.resize(t - 1);

  if (!child->isLeaf)
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

std::string BTree::to_string()
{
  std::ostringstream btree_repr;
  btree_repr << "BTree(" << to_string(root) << ")";
  return btree_repr.str();
}

std::string BTree::to_string(BTreeNodePtr node)
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
    node_repr << to_string(c);
    if (!(c == node->children.back()))
      node_repr << ",";
  }
  node_repr << "}]";
  return node_repr.str();
}

void BTree::del(int element)
{
  // We ascertain here that the tree contains the element
  // so that we don't modify the tree unnecessarily
  if (search(element).empty())
    return;

  del(root, element);
}

void BTree::del(BTreeNodePtr node, int element)
{
  int i = appropriate_index(node, element);
  // If the element to delete is contained in the node
  if (node->keys[i] == element)
  {
    if (node->isLeaf)
    {
      // The node is guaranteed to have at least t keys
      // We can simply delete the key
      node->keys.erase(node->keys.begin() + i);
      node->n--;
    }
    else
      del_internal_node(node, i, element);
  }
  else
  {
    if (node->children[i]->n < t)
      ensure_child_can_accomodate(node, i);
    del(node->children[i], element);
  }
}

int BTree::appropriate_index(BTreeNodePtr node, int element)
{
  int i = 0;
  for (; i < node->n && node->keys[i] < element; i++)
    ;
  return i;
}

void BTree::del_internal_node(BTreeNodePtr node, int i, int element)
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
      merge_children(node, i);
      // Delete the element from the merged node
      del(left_child, element);
    }
  }
}

void BTree::merge_children(BTreeNodePtr node, int index)
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

void BTree::ensure_child_can_accomodate(BTreeNodePtr node, int index)
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
    if (!to_recurse->isLeaf)
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
    if (!to_recurse->isLeaf)
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
      merge_children(node, index - 1);
    else
      merge_children(node, index);
  }
}
