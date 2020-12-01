//
// Created by via56 on 30-11-20.
//
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>



auto destroy = [](benchmark::State & state)
{// Perform setup here
    for (auto _ : state) {
        // This code gets timed
    }
};




int main (int argc, char *argv[] ){


    uint64_t len = atoi(argv[1]);
    uint64_t strLen = atoi(argv[2]);

//    benchmark::RegisterBenchmark("init",init)->Unit({benchmark::kMicrosecond});
//    benchmark::RegisterBenchmark("insert-suffixes",insert,len,strLen)->Unit({benchmark::kMicrosecond});
//    benchmark::RegisterBenchmark("feed-random",feed,strLen)->Unit({benchmark::kMicrosecond});

//    benchmark::RegisterBenchmark("search-suffixes",searchPositive,text)->Unit({benchmark::kMicrosecond});
//    benchmark::RegisterBenchmark("search-non-suffixes",searchNegative,50)->Unit({benchmark::kMicrosecond});
//    benchmark::RegisterBenchmark("destroy",destroy)->Unit({benchmark::kMicrosecond});

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;

}

