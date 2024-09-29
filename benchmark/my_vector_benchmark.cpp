//
// Created by Yile Hu on 9/29/24.
//
#include "../my_vector.h"
#include <benchmark/benchmark.h>

static void BM_VectorPushBack(benchmark::State& state) {
    my_vector<char> v(1000000, 'a');
    for (auto _ : state)
        my_vector<char> v2 = v;
}
// Register the function as a benchmark
BENCHMARK(BM_VectorPushBack);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
    std::string x = "hello";
    for (auto _ : state)
        std::string copy(x);
}
BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();