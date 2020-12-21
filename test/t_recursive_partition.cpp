
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <fstream>
#include <time.h>  
#include "../BigRRePair.h"
#include "../DummyPartition.h"
#include "../DummyRepairCompressor.h"
#include "../utils.h"
using namespace std;

//string filename = "../test_text";
string filename = "/home/via56/Documentos/doc/collections/artificial/fib41";

static void test_recursive_partition_scheme(benchmark::State & state)
{

    DummyPartition P(10,100,parseNT_exe);
    DummyRepairCompressor C(myrepair_exe);
    BigRRePair brr; 
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
        std::cout<<"compression\n";
            brr.rrpair(filename,C,P);
        std::cout<<"decompression\n";
            brr.decompress(filename);
    }

}
// Register the function as a benchmark
BENCHMARK(test_recursive_partition_scheme)->Unit(benchmark::TimeUnit::kMicrosecond);

// Run the benchmark
BENCHMARK_MAIN();