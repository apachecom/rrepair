//
// Created by via56 on 30-11-20.
//
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include "../../include/rr_trie.hpp"
#include "../UtilStrings.h"
//#define PRINT_LOG_T_TRIE
//using namespace tree;
tree::c_trie64 _trie;

auto init = [](benchmark::State & state)
{// Perform setup here
    for (auto _ : state) {
        // This code gets timed
        tree::init<tree::c_trie64,uint64_t,char>(_trie);
    }
};

auto insert = [](benchmark::State & state, const uint32_t& len,const uint32_t& strLen)
{// Perform setup here
    //build suffix-trie
    for (auto _ : state) {
        // This code gets timed
        for (uint32_t i = 0; i < len ; ++i) {
            std::string str = big_repair::util::generate_random_string(strLen)+"$";
            auto node = tree::root<tree::c_trie64,uint64_t, char>(_trie);
            for (uint32_t  j = i; j < strLen ; ++j) {
                node = tree::insert<tree::c_trie64,uint64_t, char>(node->id,( char)str[j],_trie);
            }
        }
#ifdef PRINT_LOG_T_TRIE
        tree::print<tree::c_trie64>(_trie);
        sleep(10);
#endif

    }
};

auto feed = [](benchmark::State & state, const uint32_t& strLen)
{// Perform setup here
    for (auto _ : state) {
        // This code gets timed
        auto node = tree::root<tree::c_trie64,uint64_t, char>(_trie);
        std::string s = big_repair::util::generate_random_string(strLen)+"$";
        for (uint32_t  j = 0; j < s.size() && node != nullptr ; ++j) {
            node = tree::feed(node->id,( char)s[j],_trie);
        }
    }
};

auto searchPositive = [](benchmark::State & state, const std::string& text)
{// Perform setup here
    for (auto _ : state) {
        // This code gets timed
        for (uint32_t i = 0; i < text.size() ; ++i) {
            auto node = tree::root<tree::c_trie64,uint64_t, char>(_trie);
            for (uint32_t  j = i; j < text.size() ; ++j) {
                node = tree::feed<tree::c_trie64,uint64_t, char>(node->id,( char)text[j],_trie);
                ASSERT_TRUE(node != nullptr);
            }
            bool c = tree::isLeaf<tree::c_trie64,uint64_t, char>(node->id,_trie);
            ASSERT_TRUE(c == true);
        }
    }
};
auto searchNegative = [](benchmark::State & state, const uint32_t & l)
{// Perform setup here
    for (auto _ : state) {
        // This code gets timed

        auto node = tree::root<tree::c_trie64,uint64_t, char>(_trie);

        std::string s = big_repair::util::generate_random_string(l) + "#";

        for (uint32_t  j = 0; j < s.size() && node != nullptr ; ++j) {
            node = tree::feed(node->id,( char)s[j],_trie);
        }
        bool c =  false;
        if(node != nullptr)
         c = tree::isLeaf<tree::c_trie64,uint64_t, char>(node->id,_trie);
        ASSERT_TRUE(node == nullptr || c == false );

    }

};

auto destroy = [](benchmark::State & state)
{// Perform setup here
    for (auto _ : state) {
        // This code gets timed
        tree::destroy<tree::c_trie64>(_trie);
    }
};




int main (int argc, char *argv[] ){


    uint64_t len = atoi(argv[1]);
    uint64_t strLen = atoi(argv[2]);

    benchmark::RegisterBenchmark("init",init)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("insert-suffixes",insert,len,strLen)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("feed-random",feed,strLen)->Unit({benchmark::kMicrosecond});

//    benchmark::RegisterBenchmark("search-suffixes",searchPositive,text)->Unit({benchmark::kMicrosecond});
//    benchmark::RegisterBenchmark("search-non-suffixes",searchNegative,50)->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("destroy",destroy)->Unit({benchmark::kMicrosecond});

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;

}