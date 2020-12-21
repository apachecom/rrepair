//
// Created by via56 on 14-12-20.
//



#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include "../../include/rr/rec_compresors.hpp"
#include "../../include/rr/confs.hpp"
#include "../../include/rr/compresors.hpp"
#include "../../include/utils/io.hpp"


using namespace  rr;


auto t_compress = [](benchmark::State & state, const std::string &filename, const uint32_t& w, const uint32_t& m,const uint32_t& i, bool check)
{
    rr::params input(filename,10,1e7,w,m);
    rr::mzzConf conf;
    conf.init(&input);
    rr::RePairRecursive<rr::mzzConf> R(conf);
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
            R.compress();
            std::cout<<"[1]end compression\n";
            rr::decompress(filename);
            std::cout<<"[2]end decompression\n";
            ASSERT_TRUE(io::compareFiles(filename,filename + ".rrout"));
    }
};


auto t_decompress = [](benchmark::State & state, const std::string &filename)
{
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed




    }


};


int main (int argc, char *argv[] ){

    std::string filename = argv[1];
    uint64_t w = atoi(argv[2]);
    uint64_t m = atoi(argv[3]);
    uint64_t i = atoi(argv[4]);

    benchmark::RegisterBenchmark("check-compresss",t_compress,filename,w,m,i,true)->Unit({benchmark::kMicrosecond});
//    benchmark::RegisterBenchmark("bench-compresss",t_compress,filename,w,m,i,false)->Unit({benchmark::kMicrosecond});
//    benchmark::RegisterBenchmark("bench-decompress",t_decompress,filename)->Unit({benchmark::kMicrosecond});

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;

}