//
// Created by ale on 01-07-20.
//


#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include "UtilStrings.h"
#include "../HashParser.h"
#include "../HashParserConfig.h"
#include "../RepairUtils.h"

#include "../include/rr_hash_parser.hpp"

#define LEN 100000
using namespace big_repair;
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

auto new_parse = [](benchmark::State & state, const std::string file,const std::string output_dir,uint32_t m){
    for (auto _ : state) {
        // This code gets timed
            typedef hash_parser::hParser<uint64_t,unsigned char, hash_parser::w_kr_hash_uc64 >P;
            P parser;
            parser.mod = m;
            hash_parser::compress(file,parser);
        try {
        } catch (const char * str) {
            std::cout<<str<<std::endl;
            exit(1);
        }
    }

};

auto tParseFileSM =  [](benchmark::State & state, const std::string file,const std::string output_dir,uint32_t m)
{
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
               try {
                   HashParserConfig<KRPSlindingWindow<>, KRPHashFunction<uint64_t, std::string>> conf(10, 1, m, file,"./");
                   conf.print();
                   HashParser<HashParserConfig<KRPSlindingWindow<>, KRPHashFunction<uint64_t, std::string> >> parser(
                           conf);

                   parser.parseFileSM();
                   parser.results.print();

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

                   ASSERT_TRUE(util::compareFiles(file, file+".out"));
               } catch (const char* string1) {
                    std::cout<<string1<<std::endl;
                    sleep(5);
               }
           }
};

int main (int argc, char *argv[] ){

    std::string file = argv[1];
    std::string output_dir = argv[2];
    uint32_t m = atoi(argv[3]);

    std::cout<<"file:"<<file<<std::endl;
    std::cout<<"output_dir:"<<output_dir<<std::endl;


    benchmark::RegisterBenchmark("hash-parser",tParseFileSM,file,output_dir,m)->Unit({benchmark::kMicrosecond});

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;

}