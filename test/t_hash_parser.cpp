//
// Created by ale on 01-07-20.
//


#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
//#include "UtilStrings.h"
#include "../include/utils/io.hpp"
#include "../include/utils/string.hpp"
#include "../HashParser.h"
#include "../HashParserConfig.h"
#include "../RepairUtils.h"

#include "../include/rr/rr_hash_parser.hpp"

#define LEN 100000
using namespace io;
using namespace big_repair;
using namespace utilString;
using namespace rr;
using namespace std;


//
//static void tParseFile(benchmark::State & state)
//{
//    // Perform setup here
//
//
//    for (auto _ : state) {
//        // This code gets timed
//        std::string T = util::generate_random_string(100000000);
//        std::fstream fout("tmp_it",std::ios::out);
//        fout.write(T.c_str(),T.length());
//        fout.close();
//        HashParserConfig<KRPSlindingWindow<>,KRPHashFunction<uint64_t ,std::string>> conf(10,1,10,"./tmp_it","./");
//        conf.print();
//        HashParser< HashParserConfig< KRPSlindingWindow<>,KRPHashFunction<uint64_t ,std::string> >> parser(conf);
//        parser.parseFile();
//        parser.results.print();
//        parser.recreateFile("tmp_it",1);
//        EXPECT_TRUE(util::compareFiles("tmp_it","tmp_it_recreated"));
////        sleep(3);
//
//    }
//
//}

auto byte_parse = [](benchmark::State & state, const std::string& file,const std::string output_dir,uint32_t m,uint32_t w){
    for (auto _ : state) {
        // This code gets timed


        try {
            rr::parserUC64 parser;
            rr::init(w,parser.windows);
            parser.mod = m;
            rr::compress(file,parser);
            rr::decompress(file,parser);
            ASSERT_TRUE(io::compareFiles(file, file+".out"));

        } catch (const char * str) {
            std::cout<<str<<std::endl;
            exit(1);
        }
    }

};
auto mzz_parse_bench_compression = [](benchmark::State & state, const std::string& file,const std::string output_dir,uint32_t m,uint32_t w){
    for (auto _ : state) {
        // This code gets timed
        try {
            rr::mzzParserUC64 parser;
            rr::init(w,parser.windows);
            parser.mod = m;
            rr::compress(file,parser);
        } catch (const char * str) {
            std::cout<<str<<std::endl;
            exit(1);
        }
    }
};


auto mzz_parse_bench_decompression = [](benchmark::State & state, const std::string& file,const std::string output_dir,uint32_t m,uint32_t w){
    for (auto _ : state) {
        // This code gets timed
        try {
            rr::mzzParserUC64 parser;
//            rr::init(w,parser.windows);
//            parser.mod = m;
            rr::decompress(file,parser);
        } catch (const char * str) {
            std::cout<<str<<std::endl;
            exit(1);
        }
    }
};

auto mzz_parse = [](benchmark::State & state, const std::string& file,const std::string output_dir,uint32_t m,uint32_t w){
    for (auto _ : state) {
        // This code gets timed


        try {
            rr::mzzParserUC64 parser;
            rr::init(w,parser.windows);
            parser.mod = m;
            rr::compress(file,parser);
            rr::decompress(file,parser);
//            rr::destroy(parser);
            ASSERT_TRUE(io::compareFiles(file, file+".out"));

        } catch (const char * str) {
            std::cout<<str<<std::endl;
            exit(1);
        }
    }

};
auto mzz_parse_mem_bench_compression = [](benchmark::State & state, const std::string& file,const std::string output_dir,uint32_t m,uint32_t w,
                                          const uint32_t &buff){
    for (auto _ : state) {
        // This code gets timed
        try {
            rr::mzzParserUC64 parser;
            rr::init(w,parser.windows);
            parser.mod = m;
            rr::compress_in_mem(file,parser,buff);
        } catch (const char * str) {
            std::cout<<str<<std::endl;
            exit(1);
        }
    }
};

auto mzz_parse_mem= [](benchmark::State & state,
        const std::string& file,
        const std::string output_dir,
        const uint32_t &m,
        const uint32_t &w,
        const uint32_t &buff){

    for (auto _ : state) {
        // This code gets timed
        try {
            rr::mzzParserUC64 parser;
            rr::init(w,parser.windows);
            parser.mod = m;
            rr::compress_in_mem(file,parser,buff);
            rr::decompress(file,parser);
            ASSERT_TRUE(io::compareFiles(file, file+".out"));

        } catch (const char * str) {
            std::cout<<str<<std::endl;
            exit(1);
        }
    }

};

auto tParseFileSM =  [](benchmark::State & state, const std::string& file,const std::string output_dir,uint32_t m,uint32_t w)
{
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
               try {
                   big_repair::HashParserConfig<big_repair::KRPSlindingWindow<>, KRPHashFunction<uint64_t, std::string>> conf(w, 1, m, file,"./");
//                   conf.print();
                   big_repair::HashParser<HashParserConfig<big_repair::KRPSlindingWindow<>, KRPHashFunction<uint64_t, std::string> >> parser(
                           conf);

                   parser.parseFileSM();
//                   parser.results.print();

                   std::string _f = file;
                   std::reverse(_f.begin(), _f.end());

                   int i = 0;
                   while (_f[i] != '/' && i < _f.size()) ++i;


                   std::string filename;
                   filename.resize(i + 1);
                   std::copy(_f.begin(), _f.begin() + i, filename.begin());

                   std::reverse(filename.begin(), filename.end());
//                std::cout<<output_dir+filename<<std::endl;
                   parser.recreateFile(filename, output_dir, 1);

                   ASSERT_TRUE(io::compareFiles(file, file+".out"));
               } catch (const char* string1) {
                    std::cout<<string1<<std::endl;
                    exit(1);
               }
           }
};

int main (int argc, char *argv[] ){

    std::string file = argv[1];
    std::string output_dir = argv[2];
    uint32_t m = atoi(argv[3]);
    uint32_t w = atoi(argv[4]);

//    std::cout<<"file:"<<file<<std::endl;
//    std::cout<<"output_dir:"<<output_dir<<std::endl;


//    benchmark::RegisterBenchmark("hash-parser",tParseFileSM,file,output_dir,m,w)->Unit({benchmark::kMicrosecond});
//    benchmark::RegisterBenchmark("byte-hash-parser",byte_parse,file,output_dir,m,w)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("mzz-hash-parser-check",mzz_parse,file,output_dir,m,w)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("mzz-hash-parser-in-mem-check",mzz_parse_mem,file,output_dir,m,w,1e12)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("mzz-hash-parser-compression",mzz_parse_bench_compression,file,output_dir,m,w)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("mzz-hash-parser-in-mem-compression",mzz_parse_mem_bench_compression,file,output_dir,m,w,1e12)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("mzz-hash-parser-decompression",mzz_parse_bench_decompression,file,output_dir,m,w)->Unit({benchmark::kMicrosecond});

    //    benchmark::RegisterBenchmark("mzz-hash-parser",mzz_parse,file,output_dir,m,w)->Unit({benchmark::kMicrosecond});

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;

}