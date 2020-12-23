
//
// Created by ale on 06-07-20.
//

#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <fstream>
#include <cstdlib>

#include "../HashParserConfig.h"
#include "../RePairRecursiveConfig.h"
#include "../RePairRecursive.h"
#include "../SlidingWindow.h"
#include "../mem_monitor/mem_monitor.hpp"

//#define MEM_MONITOR
#define COMPRESSOR_BIN_DIR "../external/repair/repair"

using namespace big_repair;

struct Params {

    uint32_t max_iter;
    uint32_t th;
    std::string filename;


    void help(){

        std::cout<<"usage is <filename> <max-iter> <th-len-seq>\n";
        std::cout<<"<filename> file to compress \n";
        std::cout<<"<max-iter> maximum iterations allowed\n";
        std::cout<<"<th-len-seq> threshold for the lenght of initial sequence of the parser \n";

    }

    void readParams(int argc, char *argv[]){

        if(argc < 4 ){
            help();
            throw "Wrong number of parameters";
        }

        filename = argv[1];
        max_iter = (uint32_t)std::atoi(argv[2]);
        th = (uint32_t)std::atoi(argv[3]);

    }

};


auto b_compress  = [](benchmark::State &state,const Params& params)
{

    // Perform setup here
#ifdef MEM_MONITOR
    std::string mem_out = params.filename + "-rec-repair-compression.csv";
    mem_monitor mm(mem_out);
    mm.event("compression");
#endif
    for (auto _ : state) {

        DummyRepair compresor(COMPRESSOR_BIN_DIR);

        RePairRecursiveConfig <
                uint32_t,
                DummyRepair,
                RandomPermutationParser
        > rrConf(params.filename,compresor,params.max_iter,params.th);

        RePairRecursive<
                RePairRecursiveConfig <
                        uint32_t,
                        DummyRepair,
                        RandomPermutationParser
                >
        > brepair(rrConf);

        brepair.apply();

    }
    state.counters["R-RePair-grammar-size"] = 0;
};

auto b_decompress  = [](benchmark::State &state,const Params& params)
{

    // Perform setup here
    uint32_t size = 0;
#ifdef MEM_MONITOR
    std::string mem_out = params.filename + "-rec-repair-decompression.csv";
    mem_monitor mm(mem_out);
    mm.event("decompression");
#endif

    for (auto _ : state) {
        size = util::decompress(params.filename);
    }
    state.counters["R-RePair-grammar-size"] = size;

};


int main (int argc, char *argv[] ){


    Params params;


    try {

        params.readParams(argc,argv);


        benchmark::RegisterBenchmark("Compression"  ,b_compress,params)->Unit({benchmark::kMicrosecond});
        benchmark::RegisterBenchmark("Decompression"  ,b_decompress,params)->Unit({benchmark::kMicrosecond});

        benchmark::Initialize(&argc, argv);
        benchmark::RunSpecifiedBenchmarks();


    } catch (...) {

        return 0;

    }


    return 0;

}
