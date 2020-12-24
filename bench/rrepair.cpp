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
#include "../CLogger.h"
#include "../macros.h"

#define COMPRESSOR_BIN_DIR "../external/repair/repair"

using namespace big_repair;

struct Params {

    uint32_t ws;
    uint32_t mod;
    uint32_t max_iter;
    uint32_t th;
    std::string filename;


    void help(){

        std::cout<<"usage is <filename> <ws> <mod> <max-iter> <th-len-seq>\n";
        std::cout<<"<filename> file to compress \n";
        std::cout<<"<ws> windows size used by the parser\n";
        std::cout<<"<mod> module value used by the parser\n";
        std::cout<<"<max-iter> maximum iterations allowed\n";
        std::cout<<"<th-len-seq> threshold for the lenght of initial sequence of the parser \n";

    }

    void readParams(int argc, char *argv[]){

        if(argc < 6 ){
            help();
            throw "Wrong number of parameters";
        }

        filename = argv[1];
        ws = (uint32_t)std::atoi(argv[2]);
        mod = (uint32_t)std::atoi(argv[3]);
        max_iter = (uint32_t)std::atoi(argv[4]);
        th = (uint32_t)std::atoi(argv[5]);

        CLogger::GetLogger()->model["window-size"] = ws;
        CLogger::GetLogger()->model["mod"] = mod;
        CLogger::GetLogger()->model["max-iter"] = max_iter;
        CLogger::GetLogger()->model["th"] = th;

    }

};


auto b_compress  = [](benchmark::State &state,const Params& params)
{



    // Perform setup here
#ifdef MEM_MONITOR
    std::string mem_out = params.filename + "-mem-rec-repair-compression.csv";
    mem_monitor mm(mem_out);
    mm.event("compression");
#endif



    for (auto _ : state) {

        HashParserConfig<KRPSlindingWindow<>,KRPHashFunction<uint64_t ,std::string>> conf(params.ws,1,params.mod,params.filename,"");
        DummyRepair compresor(COMPRESSOR_BIN_DIR);
        RePairRecursiveConfig <
        uint32_t,
                DummyRepair,
                HashParser< HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint64_t ,std::string > > >
                                                                                              > rrConf(conf,compresor,params.max_iter,params.th);

        RePairRecursive<RePairRecursiveConfig <
        uint32_t,
                DummyRepair,
                HashParser< HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint64_t ,std::string > > >
        >> brepair(rrConf);


#ifdef MEASURE_TIME
        {
            auto start = std::chrono::high_resolution_clock::now();
#endif
        brepair.apply();

#ifdef MEASURE_TIME
            auto end = std::chrono::high_resolution_clock::now();
            auto elapse = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            CLogger::GetLogger()->model["total:time"] = elapse;
        }
#endif



    }


#ifdef MEASURE_TIME


    auto b = CLogger::GetLogger()->model.begin();
    while(b != CLogger::GetLogger()->model.end()){
        state.counters[b->first] = b->second;//CLogger::GetLogger()->model["total:time"];
        ++b;
    }
//    state.counters["grammar:size"] = CLogger::GetLogger()->model["grammar:size"];
//    state.counters["grammar:rules"] = CLogger::GetLogger()->model["grammar:rules"];

#endif
//
//    state.counters["R-RePair-grammar-size"] = 0;
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
//        benchmark::RegisterBenchmark("Decompression"  ,b_decompress,params)->Unit({benchmark::kMicrosecond});

        benchmark::Initialize(&argc, argv);
        benchmark::RunSpecifiedBenchmarks();


    } catch (...) {

        return 0;

    }


    return 0;

}
