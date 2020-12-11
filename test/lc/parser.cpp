//
// Created by via56 on 10-12-20.
//

#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include "../../include/lc/parser.hpp"
//#include "../../include/utils/io.hpp"

auto build = [](benchmark::State & state, const std::string & file)
{
    // Perform setup here
    lc::parser<> P;

    for (auto _ : state) {

        // This code gets timed
        lc::parse_file(file,P);

        std::fstream Fout(file + ".out",std::ios::out | std::ios::binary);

        lc::decompress(Fout,P);

        ASSERT_TRUE(io::compareFiles(file,file + ".out"));

        sleep(5);
    }

//    state.counters["collisions"] = c;
//    state.counters["samples"] = samples;
//    state.counters["len"] = len;

};



int main (int argc, char *argv[] ){

    std::string coll_file = argv[1];

    benchmark::RegisterBenchmark("parser",build,coll_file)->Unit({benchmark::kMicrosecond});

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;

}
