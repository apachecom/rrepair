//
// Created by via56 on 30-11-20.
//


#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include "../../include/rr_fingerprints.hpp"
#include <unordered_map>
#include "../UtilStrings.h"
typedef fingerprints::kr_hash<char,uint64_t> kr_hash_c64;
typedef fingerprints::kr_hash<unsigned char,uint64_t> kr_hash_uc64;
typedef fingerprints::kr_hash<uint32_t ,uint64_t> kr_hash_ui64;


auto hashC64 = [](benchmark::State & state, const uint32_t& samples,const uint32_t& len)
{
    std::unordered_map<uint64_t ,std::basic_string<char>> D;
    // Perform setup here
    uint64_t c = 0;
    for (auto _ : state) {
        // This code gets timed
        c = 0;
        for (uint32_t i = 0; i < samples; ++i) {
            std::basic_string<char> str = big_repair::util::generate_random_string(len);
            kr_hash_c64 f;
            uint64_t hash = f.hash(str);
            auto it = D.find(hash);
            if(it == D.end())
                D[hash] = str;
            else{
                if(str != it->second)
                    c++;
            }
        }
    }

    state.counters["collisions"] = c;
    state.counters["samples"] = samples;
    state.counters["len"] = len;

};

auto hashUI64 = [](benchmark::State & state, const uint32_t& samples,const uint32_t& len)
{
    std::unordered_map<uint64_t ,std::basic_string<uint32_t>> D;
    // Perform setup here
    uint64_t c = 0;
    srand(time(nullptr));
    for (auto _ : state) {
        // This code gets timed
        c = 0;
        for (uint32_t i = 0; i < samples; ++i) {
            std::basic_string<uint32_t> str; str.resize(len);
            for (int j = 0; j < len; ++j)
                str[j] = rand() + 1;

            kr_hash_ui64 f;
            uint64_t hash = f.hash(str);
            auto it = D.find(hash);
            if(it == D.end()) D[hash] = str;
            else{
                if(str != it->second)
                    c++;
            }
        }
    }

    state.counters["collisions"] = c;
    state.counters["samples"] = samples;
    state.counters["len"] = len;

};




int main (int argc, char *argv[] ){

    uint64_t samples = atoi(argv[1]);
    uint64_t strLen = atoi(argv[2]);

    benchmark::RegisterBenchmark("hashC64",hashC64,samples,strLen)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("hashUI64",hashUI64,samples,strLen)->Unit({benchmark::kMicrosecond});

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;

}