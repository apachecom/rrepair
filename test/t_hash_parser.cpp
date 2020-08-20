//
// Created by ale on 01-07-20.
//


#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include "UtilStrings.h"
#include "../HashParser.h"
#include "../HashParserConfig.h"
#include "../RepairUtils.h"

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
//        std::string T = util::generate_random_string(100);
//
//        std::fstream fout("./tmp",std::ios::out);
//        fout.write(T.c_str(),T.length());
//        fout.close();
//
//
//        HashParserConfig<KRPSlindingWindow<>,KRPHashFunction<uint64_t ,std::string>> conf(10,1,10,"./tmp","./");
//
//        conf.print();
//        HashParser< HashParserConfig< KRPSlindingWindow<>,KRPHashFunction<uint64_t ,std::string> >> parser(conf);
//
//        parser.parseFile();
//
//        parser.results.print();
//
//        sleep(3);
//
//    }
//
//}
//

static void tParseFileSM(benchmark::State & state)
{
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
        std::string T = util::generate_random_string(100000000);
        std::fstream fout("tmp",std::ios::out);
        fout.write(T.c_str(),T.length());
        fout.close();
        HashParserConfig<KRPSlindingWindow<>,KRPHashFunction<uint64_t ,std::string>> conf(10,1,10,"tmp","");
        conf.print();
        HashParser< HashParserConfig< KRPSlindingWindow<>,KRPHashFunction<uint64_t ,std::string> >> parser(conf);
        parser.parseFileSM();
        parser.results.print();
        parser.recreateFile("tmp",1);
        EXPECT_TRUE(util::compareFiles("tmp","tmp_recreated"));
    }


}
//
//
//static void tPrepareForRP(benchmark::State & state)
//{
//    // Perform setup here
//    for (auto _ : state) {
//        // This code gets timed
//
//
//        std::string T = util::generate_random_string(100);
//
//        std::fstream fout("./tmp",std::ios::out);
//        fout.write(T.c_str(),T.length());
//        fout.close();
//
//
//        HashParserConfig<KRPSlindingWindow<>,KRPHashFunction<uint64_t ,std::string>> conf(10,1,10,"./tmp","./");
//
//        conf.print();
//
//        HashParser< HashParserConfig< KRPSlindingWindow<>,KRPHashFunction<uint64_t ,std::string> >> parser(conf);
//
//        parser.parseFile();
//
//        parser.results.print();
//
//        sleep(3);
//
//        util::prepareDiccFileForRP("file_dicc","file.dicc",sizeof(uint32_t),parser.results._max_alph_val);
//
//
//
//
//    }
//
//}


static void firstTest(benchmark::State & state)
{
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
    }

}
// Register the function as a benchmark
BENCHMARK( firstTest)->Unit(benchmark::TimeUnit::kMicrosecond);
//BENCHMARK( tParseFile)->Unit(benchmark::TimeUnit::kMicrosecond);
BENCHMARK( tParseFileSM)->Unit(benchmark::TimeUnit::kMicrosecond);
//BENCHMARK( tPrepareForRP)->Unit(benchmark::TimeUnit::kMicrosecond);

// Run the benchmark
BENCHMARK_MAIN();