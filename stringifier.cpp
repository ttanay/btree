#include "btree.h"

#include <iostream>
#include <string>

using namespace std;

string print_btree(int minimum_degree, vector<int> input_stream)
{
  BTree btree = BTree(minimum_degree, input_stream);
  return btree.to_string();
}

int main(int argc, char **argv)
{
  int minimum_degree, input_size;
  vector<int> input_stream;

  cout << "BTree Stringifier: See string representation of a BTree" << endl;
  cout << "-------------------------------------------------------" << endl;

  cout << "Enter minimum_degree(t) of BTree: ";
  cin >> minimum_degree;
  cout << endl;

  cout << "Enter size of input stream: ";
  cin >> input_size;
  cout << endl;

  cout << "Enter input to construct the BTree: ";
  for (int i = 0; i < input_size; i++)
  {
    int e;
    cin >> e;
    input_stream.push_back(e);
  }
  cout << endl;
  cout << print_btree(minimum_degree, input_stream) << endl;
  return 0;
}
