//
// Created by ale on 01-07-20.
//


#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include "../HashFunction.h"
#include "UtilStrings.h"


using namespace big_repair;
using namespace std;


static void firstTest(benchmark::State & state)
{
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
    }

}


static void tKRPHashFunction(benchmark::State & state)
{
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
        KRPHashFunction <uint64_t ,std::string> f;
        std::string s = util::generate_random_string(10);
        std::cout<<"string:"<<s<<"/"<<f.apply(s)<<std::endl;
    }

}
// Register the function as a benchmark
BENCHMARK( firstTest)->Unit(benchmark::TimeUnit::kMicrosecond);
BENCHMARK( tKRPHashFunction)->Unit(benchmark::TimeUnit::kMicrosecond);

// Run the benchmark
BENCHMARK_MAIN();