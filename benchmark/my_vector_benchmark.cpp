//
// Created by Yile Hu on 9/29/24.
//
#include "../include/Vector.h"
#include <benchmark/benchmark.h>

static void BM_VectorPushBack(benchmark::State& state) {
    mystl::Vector<int> v;
    for (auto _ : state)
        v.push_back(1);
}

// Register the function as a benchmark
BENCHMARK(BM_VectorPushBack);
static void BM_STLVectorPushBack(benchmark::State& state) {
    std::vector<int> v;
    for (auto _ : state)
        v.push_back(1);
}
BENCHMARK(BM_STLVectorPushBack);
BENCHMARK_MAIN();