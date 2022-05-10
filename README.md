# Notes from Cormen
The size of a node in a B-Tree is as large as the size of a whole page on disk.
This is because we want to read as much data as possible in a single disk seek.

A large branching factor reduces the height of the tree and the no of disk accesses
required to find any key.

The essence of a B-Tree, is that given an element `i`, it will search through an array,
if the element is not found, it gives the location of another node, where this search can be
done based on the condition that the location contains all elements in a reduced set of which
`i` is a member.
So, it recursively reduces the search space by a factor of `t` until the element is found.

## Definition
Node(x) Attributes:
1. `n`, the no of keys stored in the node currently
2. `keys`, an array of size `n` stored sorted and in ascending order
3. `leaf?`, whether the node is a leaf node
4. `children`, an array of size `n+1` pointers to the child nodes. Leaf nodes have this as NIL

Properties:
1. Given a node `x`, any key has a child node `x.c`<sub>`i`</sub>
2. All leaves have the same height.
3. 1. `t` is the minimum degree for a B-Tree. `t >= 2`
   It is the minimum no of child nodes that a node can have
    For non-root nodes:
    2.1.`t-1` <= `x.n` <= `2t - 1`
    2.2 `t` <= `size(x.c)` <= `2t`
    For root nodes:
    3.1  `1` <= `x.n` <= `2t - 1`
    3.2  `2` <= `size(x.c)` <= `2t`
4. The height of a tree is given by:
    `h <= log(((n + 1) / 2), t)`


## Search
`search(T.root, k)`, where `T` is the B-Tree, and `k` is the key we are searching for.
Result = `(Node, index)`, where Node is the pointer to the node in which the key resides, and index is the index of the key in the node, such that, `Node.key[index] = k`

Overview:
1. Find appropriate index where the key is likely to be found
2. Check if the key is found
3. Recursively search the subtree if not a leaf node. Since we are already at the index where the key can be found, we can look at the corresponding child node.

## Insert
We insert a new node in the appropriate lead node.
Since, simply inserting it at the leaf node would break the properties (3.2.1) of a B-Tree,
we split the node at the median key, and move it up to the parent node.
We perform this split operation recursively on the parent node, until the parent node is NIL.

This introduces two parts to the insertion algorithm - search for right place, node split.
To perform the operation in a single pass, we split each full node we come along the way,
so that we are assured that that each node's parent is not full.
This way, we avoid doing two passes on the tree for insertion.

Note: Handle the case where the root node is full separately before traversing the tree.

1. If x is a leaf node:
  1. Copy over keys and children to `i+1`, where `i` is the index where the key needs to be inserted.
  2. Assign key and child pointer new values at `i`.
2. else
  1. Find index `i` where the node needs to be inserted
  2. Split child node if full
  3. Recurse to child


### Node Split
Given a non-full internal node `x`, and index `i`such that `x.ci` is a full child(n = 2t - 1).
1. Create empty node. It is intended to be at the same level as the full node.
2. Copy keys and children to new node
3. Reduce `n` of original node, to indicate new node size; (delete extra keys,pointers)
4. Resize current node, by moving the child node pointers and keys right by one index.
5. Assign key in current node the median key from old child
6. Increment size of B-Tree node


## Delete
Given a tree rooted at `x`, key to remove from the tree - `k`.
Given that child `y` that precedes `k` such that `x.keys[i] = k` and `x.children[i] = y`.
Given that child `z` that succeeds `k` such that `x.keys[i] = k` and `x.children[i+1] = z`.
1. If `k` is in `x`
  1. If `x` is a lead node:
    1. Delete `k` from `x`.
  2. If `x` is an internal node:
    1. If `y.n >= t`:
      1. Find `k'` such that `k' = predecessor(k)`.
      2. Recursively delete `k'`.
      3. Replace `k` by `k'` in `x`.
    2. If `y.n < t`
      1. Find `k'` such that `k' = successor(k)`.
      2. Recursively delete `k'`.
      3. Replace `k` by `k'` in `x`.
    3. If `y.n <= z.n < t`:
      1. Merge `k` and all of `z` into `y` such that `x.keys[i] != k` and `x.children[i+1] != z` and `y.n = 2t - 1`.
      2. Free `z`
      3. Recursively delete `k` from `y`
2. Else(`k` not in `x`):
  1. Find `i` such that `x.keys[i-1] < k < x.keys[i]`
  2. If `x.children[i].n < t` and (`x.children[i-1].n >= t` or `x.children[i+1].n >= t`)
    1. Move key from `x` down to `x.children[i]`
    2. Move key from `x.children[i]`'s immediate left or right sibling up into `x`.
    3. Move appropriate child pointer from sibling into `x.children[i]`.
  3. If `x.children[i].n < t` and (`x.children[i-1].n < t` and `x.children[i+1].n < t`):
    1. Merge `x.children[i]` with one sibling
    2. Move a key from `x` down into merged node to become median node.
  4. Recurse


### Primitives
1. **Predecessor**: In the sorted list of the BTree's keys, get the predecessor of the given key according to the sorted order.
This is done by recursing to the left-most leaf node of the left sub-tree.
2. **Successor**: In the sorted list of the BTree's keys, get the successor of the given key according to the sorted order.
This is done by recursing to the right-most leaf node of the right sub-tree.
3. **MergeChildren**: When both children are low on keys(`n < t`), merge the child nodes into a single node with the key from the node as the median key.

# Observations
1. B-Tree can have sparse levels. Since the cost of search is O(log(h)),
the cost of search will be less if the no of levels/height is low with dense nodes.

# TODO
- [X] Add support for Delete
- [ ] Refactor and cleanup code
- [X] Figure out build system
