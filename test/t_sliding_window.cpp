//
// Created by ale on 01-07-20.
//


#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include "../HashFunction.h"
#include "../SlidingWindow.h"
#include "UtilStrings.h"

using namespace big_repair;
using namespace std;


static void tKRPSlindingWindow(benchmark::State & state)
{
    // Perform setup here

    KRPHashFunction <uint64_t ,std::string> f;

    for (auto _ : state) {
        // This code gets timed
        std::string T = util::generate_random_string(100);
        // init empty KR window: constructor only needs window size
        uint ii =  0;
        KRPSlindingWindow<> krw(100);
        for (char i : T) {
            int c = (int)i;
            uint64_t hash = krw.hashAddCharToWindow(c);
            std::string s; s.resize(++ii);
            std::copy(T.begin(),T.begin()+ii,s.begin());
            uint64_t hash_krp = f.apply(s);
//            std::cout<<"w:"<<hash<<"/f:"<<hash_krp<<std::endl;
            EXPECT_EQ(hash,hash_krp);
        }
        krw.reset();
    }

}


static void firstTest(benchmark::State & state)
{
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
    }

}
// Register the function as a benchmark
BENCHMARK( firstTest)->Unit(benchmark::TimeUnit::kMicrosecond);
BENCHMARK( tKRPSlindingWindow)->Unit(benchmark::TimeUnit::kMicrosecond);

// Run the benchmark
BENCHMARK_MAIN();