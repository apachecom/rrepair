//
// Created by ale on 03-07-20.
//


#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include "UtilStrings.h"
#include "../HashParserConfig.h"
#include "../RePairRecursiveConfig.h"
#include "../RePairRecursive.h"

using namespace big_repair;
using namespace std;

static void tRecursiveRePair(benchmark::State & state)
{


    // Perform setup here
    for (auto _ : state) {


        std::string T = util::generate_random_string(10000);

        std::fstream fout("./tmp",std::ios::out);
        fout.write(T.c_str(),T.length());
        fout.close();


        HashParserConfig<KRPSlindingWindow<>,KRPHashFunction<uint64_t ,std::string>> conf(10,1,10,"./tmp","./");
        DummyRepair compresor("../external/repair/repair");
        RePairRecursiveConfig <
                uint32_t,
                DummyRepair,
                HashParser< HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint64_t ,std::string > > >
        > rrConf(conf,compresor,10,4);

        RePairRecursive<RePairRecursiveConfig <
                uint32_t,
                DummyRepair,
                HashParser< HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint64_t ,std::string > > >
        >> brepair(rrConf);

        brepair.apply();
        sleep(1);

        // This code gets timed
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
BENCHMARK( tRecursiveRePair )->Unit(benchmark::TimeUnit::kMicrosecond);
// Run the benchmark
BENCHMARK_MAIN();