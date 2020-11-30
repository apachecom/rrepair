//
// Created by via56 on 29-11-20.
//

#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include "../../include/rr_utils.hpp"





auto testComputeLowerPrime = [](benchmark::State & state, const uint64_t& limit)
{// Perform setup here
    std::cout<<"testComputeLowerPrime("<<limit<<")"<<std::endl;
    for (auto _ : state) {
        // This code gets timed
        uint64_t prime = utils::computeLowerPrime<uint64_t>(limit);
        for (uint64_t i = 2; i < prime ; ++i) {
            ASSERT_TRUE(prime%i != 0);
        }
    }

};



int main (int argc, char *argv[] ){
    uint64_t limit = 1ULL << 32;
    benchmark::RegisterBenchmark("computeLowerPrime"  ,testComputeLowerPrime,limit)->Unit({benchmark::kMicrosecond});
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;

}