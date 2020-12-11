//
// Created by via56 on 02-12-20.
//

#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include "../UtilStrings.h"
#include "../../RepairUtils.h"
#include "../../include/rr/hash_parser.hpp"

#define LEN 100000
using namespace big_repair;
using namespace std;

auto byte_parse = [](benchmark::State & state, const std::string& file,const std::string output_dir,uint32_t m){
    for (auto _ : state) {
        // This code gets timed

//            hash_parser::parserUC64 parser;
//            parser.mod = m;
//            hash_parser::compress(file,parser);

        hash_parser::AI k;
        hash_parser::m1(k);
        try {
        } catch (const char * str) {
            std::cout<<str<<std::endl;
            exit(1);
        }
    }

};

int main (int argc, char *argv[] ){

    std::string file = argv[1];
    std::string output_dir = argv[2];
    uint32_t m = atoi(argv[3]);

    std::cout<<"file:"<<file<<std::endl;
    std::cout<<"output_dir:"<<output_dir<<std::endl;

    benchmark::RegisterBenchmark("byte-hash-parser",byte_parse,file,output_dir,m)->Unit({benchmark::kMicrosecond});

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;

}