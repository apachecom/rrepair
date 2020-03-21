#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <fstream>
#include <time.h>
#include "../BigRRePair.h"
#include "../DummyPartition.h"
#include "../DummyRepairCompressor.h"
#include "../utils.h"

using namespace std;

auto bigrepair_old  = [](benchmark::State &state,const std::string& file)
{
    /*
     * Perfome setup here
     *
     * */

#ifdef DEBUG_LOG_M
    CLogger::GetLogger()->model["calls_to_partitioner"] = 0;
//    CLogger::GetLogger()->model["coll"] = file;
#endif
    size_t G = 0;
    for (auto _ : state)
    {
        /*
         * This code gets timed
         *
         * */

        std::string nm_cmd_bigrepair = "../repos/bigrepair/bigrepair "+file;

        if(std::system(nm_cmd_bigrepair.c_str()) < 0 )
        {
            std::cout<<"Error we can't exe bigrepair on files\n";
            std::cout<<"Error:"<<file<<std::endl;
            sleep(10);
            return 0;
        }

    }

//    CLogger::GetLogger()->write_stats();


    state.counters["th"] = RR_SIZE;
    state.counters["G"] = G;

#ifdef DEBUG_LOG_M
    for(auto && m:CLogger::GetLogger()->model)
        state.counters[m.first] = 0;
#endif

};


auto b_compress  = [](benchmark::State &state,const std::string& file)
{
    /*
     * Perfome setup here
     *
     * */

    DummyPartition P(10,10);
    DummyRepairCompressor C(myrepair_exe);
    BigRRePair brr;

#ifdef DEBUG_LOG_M
    CLogger::GetLogger()->model["calls_to_partitioner"] = 0;
//    CLogger::GetLogger()->model["coll"] = file;
#endif

    size_t G = 0;
    for (auto _ : state)
    {
        /*
         * This code gets timed
         *
         * */
        G = brr.rrpair(file,C,P);

    }

//    CLogger::GetLogger()->write_stats();


    state.counters["th"] = RR_SIZE;
    state.counters["G"] = G;

#ifdef DEBUG_LOG_M
    for(auto && m:CLogger::GetLogger()->model)
        state.counters[m.first] = m.second;
#endif
};



int main (int argc, char *argv[] ){

    if(argc < 2){
        std::cout<<"bad parameters....";
        return 0;
    }
    std::string collection = argv[1];

    std::fstream infile(collection,std::ios::in);

    std::string line;
    while (std::getline(infile, line))
    {

        fstream f(line,std::ios::in);
        if(!f.is_open()){
            std::cout<<"error opening "+line;
            sleep(10);
            continue;
        }
        benchmark::RegisterBenchmark("run_collection_bigrepair_rec_partition",b_compress,line)->Unit({benchmark::kMicrosecond});

//        benchmark::RegisterBenchmark("run_collection_bigrepair",bigrepair_old,line)->Unit({benchmark::kMicrosecond});

    }


    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

}
