# BTree
Implementation of a BTree as described in Introduction to Algorithms[1].
I made this to get back into C++ with a non-trivial project.

This includes tests for the implementation as well as a tool(`stringifier.cpp`)
to print the string representation of a BTree.

To see details of the data structure implemented, please see [my notes](./notes.md).

# How to run
This uses CMake to build the project. Run:
```bash
cmake -B ./build .
make -C ./build/
```
This will create two binaries - `btree_stringifier` and `btree_test`.

# TODO
- [X] Add support for Delete
- [ ] Refactor and cleanup code
- [X] Figure out build system

# Reference
1. Cormen, T. H., Leiserson, C. E., Rivest, R. L.,, Stein, C. (2001). Introduction to Algorithms. The MIT Press. ISBN: 0262032937
