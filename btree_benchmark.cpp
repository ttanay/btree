// BTree benchmarks
// 1. Insert
// 2. Delete
// 3. Search

#include <vector>
#include <benchmark/benchmark.h>

#include "btree.h"

static void BM_BtreeCreation(benchmark::State & state)
{
  for (auto _ : state)
    btree::BTree{2, std::vector<int>{1, 3, 5, 4, 7, 8, 9, 2, 6, 10, 12}};
}

BENCHMARK(BM_BtreeCreation);

BENCHMARK_MAIN();
