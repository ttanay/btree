#include "btree.h"

#include <iostream>
#include <string>


std::string print_btree(int minimum_degree, std::vector<int> input_stream) {
  BTree btree = BTree(minimum_degree);
  for(auto i: input_stream)
    btree.insert(i);
  return btree.to_string();
}

int main(int argc, char** argv) {
  int minimum_degree, input_size;
  std::vector<int> input_stream;

  std::cout << R"(
_____________________                       ____   ____.__                    .__  .__
\______   \__    ___/______   ____   ____   \   \ /   /|__| ________ _______  |  | |__|_______ ___________
 |    |  _/ |    |  \_  __ \_/ __ \_/ __ \   \   Y   / |  |/  ___/  |  \__  \ |  | |  \___   // __ \_  __ \
 |    |   \ |    |   |  | \/\  ___/\  ___/    \     /  |  |\___ \|  |  // __ \|  |_|  |/    /\  ___/|  | \/
 |______  / |____|   |__|    \___  >\___  >    \___/   |__/____  >____/(____  /____/__/_____ \\___  >__|
        \/                       \/     \/                     \/           \/              \/    \/
  )" << std::endl;

  std::cout << "Enter minimum_degree(t) of BTree: ";
  std::cin >> minimum_degree;
  std::cout << std::endl;

  std::cout << "Enter size of input stream: ";
  std::cin >> input_size;
  std::cout << std::endl;

  std::cout << "Enter input to construct the BTree: ";
  for(int i = 0; i < input_size; i++) {
    int e;
    std::cin >> e;
    input_stream.push_back(e);
  }
  std::cout << std::endl;
  std::cout << print_btree(minimum_degree, input_stream) << std::endl;
  return 0;
}
