// BTree benchmarks
// 1. Insert
// 2. Delete
// 3. Search

#include <algorithm>
#include <numeric>
#include <random>
#include <vector>
#include <benchmark/benchmark.h>

#include "btree.h"

const int BTREE_SIZE = 1000;
std::vector<int> generate_input_stream(int btree_size)
{
  std::vector<int> input(btree_size);
  std::iota(input.begin(), input.end(), 1);

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(input.begin(), input.end(), g);
  return input;
}

static void BM_BtreeCreation(benchmark::State & state)
{
  std::vector<int> input = generate_input_stream(BTREE_SIZE);
  int degree = state.range(0);
  for (auto _ : state)
    btree::BTree{degree, input};
}

BENCHMARK(BM_BtreeCreation)->RangeMultiplier(2)->Range(2, 2 << 6);

BENCHMARK_MAIN();
