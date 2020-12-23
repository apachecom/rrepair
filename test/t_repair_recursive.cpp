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

#define TSIZE 1000000
#define WINDOWS_SIZE 5
#define MOD 5
#define COMPRESSOR_BIN_DIR "../external/repair/repair"
#define MAX_ITER 10
#define TH_INITIAL_SEQ 100000


static void tRecursiveRePair(benchmark::State & state)
{


    // Perform setup here
    for (auto _ : state) {


            std::string T = util::generate_random_string(TSIZE);

            std::fstream fout("tmp_1",std::ios::out);
            fout.write(T.c_str(),T.length());
            fout.close();


            HashParserConfig<KRPSlindingWindow<>,KRPHashFunction<uint64_t ,std::string>> conf(WINDOWS_SIZE,1,MOD,"tmp_1","");
            DummyRepair compresor(COMPRESSOR_BIN_DIR);
            RePairRecursiveConfig <
                    uint32_t,
                    DummyRepair,
                    HashParser< HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint64_t ,std::string > > >
            > rrConf(conf,compresor,MAX_ITER,TH_INITIAL_SEQ);

            RePairRecursive<RePairRecursiveConfig <
                    uint32_t,
                    DummyRepair,
                    HashParser< HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint64_t ,std::string > > >
            >> brepair(rrConf);

            brepair.apply();

            uint32_t size = util::decompress("tmp_1");


            ASSERT_TRUE(util::compareFiles("tmp_1","tmp_1_dec"));


            std::cout<<"R-RePair-grammar-size:"<<size<<std::endl;
            std::cout<<"Text-size:"<<TSIZE<<std::endl;
            std::cout<<"Compression-ratio:"<<100.0 - size*100.0/(TSIZE*1.0)<<"%"<<std::endl;
            std::cout<<"alph-size:"<<util::charset().size()<<std::endl;
            std::cout<<"win-size:"<<WINDOWS_SIZE<<std::endl;
            std::cout<<"module-param:"<<MOD<<std::endl;
            std::cout<<"max-iter:"<<MAX_ITER<<std::endl;
            std::cout<<"th-inital-seq:"<<TH_INITIAL_SEQ<<std::endl;

    }
}


static void tRecursiveRePairRandomParser(benchmark::State & state)
{


    // Perform setup here
    for (auto _ : state) {


        std::string T = util::generate_random_string(TSIZE);

        std::fstream fout("tmp_2",std::ios::out);
        fout.write(T.c_str(),T.length());
        fout.close();


        DummyRepair compresor(COMPRESSOR_BIN_DIR);
        RePairRecursiveConfig <
                uint32_t,
                DummyRepair,
                RandomPermutationParser
        > rrConf("tmp_2",compresor,MAX_ITER,TH_INITIAL_SEQ);

        RePairRecursive<
                RePairRecursiveConfig <
                    uint32_t,
                    DummyRepair,
                    RandomPermutationParser
                >
        > brepair(rrConf);

        brepair.apply();

        uint32_t size = util::decompress("tmp_2");


        ASSERT_TRUE(util::compareFiles("tmp_2","tmp_2_dec"));


        std::cout<<"R-RePair-grammar-size:"<<size<<std::endl;
        std::cout<<"Text-size:"<<TSIZE<<std::endl;
        std::cout<<"Compression-ratio:"<<100.0 - size*100.0/(TSIZE*1.0)<<"%"<<std::endl;
        std::cout<<"alph-size:"<<util::charset().size()<<std::endl;
        std::cout<<"win-size:"<<WINDOWS_SIZE<<std::endl;
        std::cout<<"module-param:"<<MOD<<std::endl;
        std::cout<<"max-iter:"<<MAX_ITER<<std::endl;
        std::cout<<"th-inital-seq:"<<TH_INITIAL_SEQ<<std::endl;

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
BENCHMARK( tRecursiveRePairRandomParser )->Unit(benchmark::TimeUnit::kMicrosecond);
// Run the benchmark
BENCHMARK_MAIN();