#include "btree.h"

#include <iostream>
#include <string>

using namespace std;

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
  cout << BTree(minimum_degree, input_stream).toString() << endl;
  return 0;
}
