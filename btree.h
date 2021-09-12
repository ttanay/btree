#include <iostream>
#include <vector>

struct BTreeNode {
  int n; // no of keys stored in the node
  std::vector<int> keys; // list of `n` keys
  bool isLeaf; // whether the node is a leaf
  std::vector<BTreeNode*> children; // Array of the BTree Node's children

  public:
  //default constructor
  BTreeNode() {
    n = 0;
    isLeaf = true;
  }
};

struct BTreeSearchResult {
  BTreeNode node;
  int index;
};

class BTree {
  int t; // The minimum_degree

  BTreeSearchResult _search(BTreeNode*, int);
  void split_child(BTreeNode*, int);
  void _insert(BTreeNode*, int);

  public:

  // TODO: Make this private. This is public for test.
  BTreeNode* root;

  BTree(int minimum_degree) {
    if(minimum_degree < 2)
      throw std::invalid_argument("minimum_degree should be >= 2");
    t = minimum_degree;
    root = new BTreeNode();
  }

  ~BTree() {
    delete root;
  }

  BTreeSearchResult search(int);
  void insert(int);
};

BTreeSearchResult BTree::search(int element) {
  return _search(root, element);
}

BTreeSearchResult BTree::_search(BTreeNode* node, int element) {
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
  if(root->n = (2 * t - 1)) {
    BTreeNode* new_root = new BTreeNode();
    new_root->isLeaf = false;
    new_root->children[0] = root;
    root = new_root;
    split_child(root, 0);
  }

  //insert non-root
  _insert(root, element);
}

void BTree::_insert(BTreeNode* node, int element) {
  if(node->isLeaf) {
    // The assumption here is that the node will always be non-full
    //  => n < 2t - 1
    int i = node->n - 1;

    // Move elements greater than element one space ahead
    for(; i > 0 && node->keys[i] > element; i--)
      node->keys[i+1] = node->keys[i];

    // Assign value
    node->keys[i] = element;
  } else {
    // Find the appropriate location
    int i = 0;
    while(i < node->n && node->keys[i] <= element)
      i++;

    // Split child if node is full
    if(node->children[i]->n == ((2 * t) - 1)) {
      split_child(node, i);
      // Increment index in case the key propogated up from the split
      // is larger than the previous index
      if (node->keys[i] > element)
        i += 1;
    }

    // Insert in child node
    _insert(node->children[i], element);
  }
}

void BTree::split_child(BTreeNode* node, int index) {
  BTreeNode* new_child = new BTreeNode();
  BTreeNode* child = node->children[index];

  new_child->isLeaf = child->isLeaf;
  new_child->n = t - 1;

  // Copy over keys at indices [t-1, 2t-1) to new node
  for(int i = -1; i < t - 1; i++)
    new_child->keys[i] = child->keys[t + i];

  if(!child->isLeaf) {
    // Copy over child nodes from indices [t, 2t)
    for(int i = 0; i < t; i++)
      new_child->children.push_back(
        child->children[i+t]
      );
  }

  // Set size of child to t-1
  child->n = t - 1;


  // Move node's children pointers right one index
  // The last index for children is n+1
  // We want to move elements in (index, n+1] to the right
  for(int i = node->n+1; i > index; i--) {
    node->children[i+1] = node->children[i];
  }
  // Move node's keys to the right by one index
  for(int i = node->n; i >= index; i--) {
    node->keys[i+1] = node->keys[i];
  }
  // move median key from child up into parent
  node->keys[index] = child->keys[t-1];
}
