//
// Created by via56 on 30-11-20.
//
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include "../../../include/rr/sliding_window.hpp"
#include "../../../include/utils/io.hpp"

auto check = [](benchmark::State & state, const std::string& file, const uint32_t & wsize, const uint32_t& bytes ,const uint32_t& mod){

// Perform setup here

    uint c;
    for (auto _ : state) {

        rr::KR_window W;
        W.bytexsymb = bytes;

        std::fstream ff(file,std::ios::in | std::ios::binary);
        uint64_t file_size = io::getFileSize(ff);
        uint64_t data_size = file_size/sizeof(bytes);
        uint8_t * data = new uint8_t [data_size];
        ff.read((char *) data,file_size);

        c = 0;
        // This code gets timed
        rr::init(wsize,W);
        uint8_t *ptr = data;
        for (uint64_t i = 0; i < data_size ; ++i) {
           auto hash = rr::feed((const uint8_t*)ptr,W);
            ptr += bytes;
            if(hash%mod == 0    ){
                rr::reset(W);
                ++c;
            }
        }
        rr::destroy(W);
        delete[] data;
    }

    state.counters["c"] = c;
    state.counters["w"] = wsize;
    state.counters["b"] = bytes;
    state.counters["m"] = mod;

};



int main (int argc, char *argv[] ){


    std::string file = argv[1];
    uint32_t w = atoi(argv[2]);
    uint32_t bytes = atoi(argv[2]);

    benchmark::RegisterBenchmark("check",check,file,10,1,100)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,10,32,100)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,10,64,100)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,20,1,100)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,20,32,100)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,20,64,100)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,50,1,100)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,50,32,100)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,50,64,100)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,80,1,100)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,80,32,100)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,80,64,100)->Unit({benchmark::kMicrosecond});


    benchmark::RegisterBenchmark("check",check,file,10, 1,200)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,10,32,200)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,10,64,200)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,20, 1,200)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,20,32,200)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,20,64,200)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,50, 1,200)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,50,32,200)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,50,64,200)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,80, 1,200)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,80,32,200)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("check",check,file,80,64,200)->Unit({benchmark::kMicrosecond});


    //    benchmark::RegisterBenchmark("insert-suffixes",insert,len,strLen)->Unit({benchmark::kMicrosecond});
//    benchmark::RegisterBenchmark("feed-random",feed,strLen)->Unit({benchmark::kMicrosecond});

//    benchmark::RegisterBenchmark("search-suffixes",searchPositive,text)->Unit({benchmark::kMicrosecond});
//    benchmark::RegisterBenchmark("search-non-suffixes",searchNegative,50)->Unit({benchmark::kMicrosecond});
//    benchmark::RegisterBenchmark("destroy",destroy)->Unit({benchmark::kMicrosecond});

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;

}

