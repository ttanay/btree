#include "btree.h"

#include <sstream>


bool BTree::is_full(BTreeNodePtr node) {
  return node->n == 2*t - 1;
}

BTreeSearchResult BTree::search(int element) {
  return _search(root, element);
}

BTreeSearchResult BTree::_search(BTreeNodePtr node, int element) {
  int i = 0;
  // Find appropriate index where the key is likely to be found
  while(i < node->n && node->keys[i] < element)
    i++;

  if(node->keys[i] == element)
    return BTreeSearchResult {*node, i};
  else if(node->isLeaf)
    return BTreeSearchResult {};
  else
    return _search(node->children[i], element);
}

void BTree::insert(int element) {
  if(is_full(root))
    split_root();

  //insert non-root
  _insert(root, element);
}

void BTree::split_root() {
  BTreeNodePtr new_root = std::make_shared<BTreeNode>();
  new_root->isLeaf = false;
  new_root->children.insert(new_root->children.begin(), root);
  root = new_root;
  split_child(root, 0);
}

void BTree::_insert(BTreeNodePtr node, int element) {
  if(node->isLeaf) {
    // The assumption here is that the leaf node will always be non-full
    //  => n < 2t -1
    int i = node->n - 1;
    // Move keys one place to the right to make space to insert the element
    for(; i >= 0 && node->keys[i] > element ; i--);
    //   node->keys.insert(node->keys.begin() + i + 1, node->keys[i]);

    // // Insert at the correct index
    // node->keys.emplace(node->keys.begin() + i + 1, element);
    // Increment size
    node->keys.insert(node->keys.begin() + i + 1, element);
    node->n += 1;
  } else {
    // Find the appropriate location

    int i = node->n - 1;
    for(;i >= 0 && node->keys[i] > element; i--);
    // The appropriate place will be the right child of the key
    i += 1;

    // Split child if node is full
    if(node->children[i]->n == (2 * t - 1)) {
      split_child(node, i);
      // Increment index in case the key propogated up from the split
      // is larger than the previous index
      if (node->keys[i] < element)
        i += 1;
    }

    // Insert in child node
    _insert(node->children[i], element);
  }
}


// Observation: Since BTreeNodePtr is a std::shared_ptr, it calls the copy
// constructor on every method call
void BTree::split_child(BTreeNodePtr node, int index) {
  BTreeNodePtr new_child = std::make_shared<BTreeNode>();
  BTreeNodePtr child = node->children[index];

  new_child->isLeaf = child->isLeaf;
  new_child->n = t - 1;

  // Copy over keys at indices [t-1, 2t-1) to new node
  new_child->keys.insert(new_child->keys.begin(), child->keys.begin() + t, child->keys.end());

  // Remove excess keys from the node
  child->keys.resize(t-1);

  if(!child->isLeaf) {
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
  for(int i = node->n; i > index + 1; i--)
    node->children.insert(node->children.begin() + i + 1, node->children[i]);

  // Make the new child node the child of the parent
  node->children.emplace(node->children.begin() + index + 1, new_child);

  // Move node's keys to the right by one index
  for(int i = node->n - 1; i >= index; i--)
    node->keys.insert(node->keys.begin() + i + 1, node->keys[i]);

  // move median key from child up into parent
  node->keys.emplace(node->keys.begin() + index, child->keys[t-1]);

  // Increment size of keys in `node`
  node->n += 1;
}

std::string BTree::to_string() {
  std::ostringstream btree_repr;
  btree_repr << "BTree(" << _to_string(root) << ")";
  return btree_repr.str();
}


// [{}, []]
std::string BTree::_to_string(BTreeNodePtr node) {
  std::ostringstream node_repr;
  node_repr << "[keys={";
  for(auto i: node->keys) {
    node_repr << i;
    if(!(i == node->keys.back()))
      node_repr << ",";
  }
  node_repr << "},children={";
  for(auto c: node->children) {
    node_repr << _to_string(c);
    if(!(c == node->children.back()))
      node_repr << ",";
  }
  node_repr << "}]";
  return node_repr.str();
}
