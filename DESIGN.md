# B-Tree
A custom implementation of a B-Tree.

## Design
Each node in the B-Tree has keys.
Each node in the B-Tree also has children nodes which is 1 more than the keys in the node.
They nodes and they keys respect the property:
For a key `k` in a given node at index `i`, with subtrees given by `STi`, where `i` is the index of the tree inside the node, then:
  keys(STi) <= k < keys(STi+1), where keys(STi) gives the keys in a subtree at index i.

Rules:
1. All leaves are at the same level
2. Tree is defined by a setting - `MINIMUM_DEGREE`, which is defined by the block size. [Why? Refer Database Internals]
3. Three classes of nodes in tree - Root, Internal, Leaf.
4. Every non-root node must contain >= `ceil((MINIMUM_DEGREE - 1) / 2)` keys.
5. Root contains >1 keys.
6. All nodes contain no of keys <= `MINIMUM_DEGREE - 1` keys
7. Insertion of a node only happens at leaf node.


MINIMUM` - The minimum no of nodes in a single node

No of subtrees of non-leaf node = No of elements in node + 1
For non-leaf nodes

```
enum NodeType{ROOT, INTERNAL, LEAF};

class Node {
  int* keys;
  Node* subtrees;
  NodeType node_type;
};

class BTree {
  Node* head
  int minimum_degree
};
```


### Search:
Assumptions:
1. B-Tree is valid
```
Search(tree, element)
  found <- false
  node = tree.head // Will be nullptr if unassigned.
  while(node)
    // Binary Search on node
    index, found = binary_search(node->keys)
    if found == true
      return node->keys[index]
    else
      node = node->

```
