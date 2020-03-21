//
// Created by alejandro on 04-12-19.
//

#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <fstream>
#include <time.h>
#include "../BigRRePair.h"
#include "../DummyPartition.h"
#include "../DummyRepairCompressor.h"
#include "../utils.h"

using namespace std;

auto repair= [](benchmark::State &state,const std::string& file)
{
    /*
     * Perfome setup here
     *
     * */
    for (auto _ : state)
    {
        /*
         * This code gets timed
         *
         * */
        std::string nm_cmd_bigrepair = "../repos/bigrepair/myrepair/repair "+file;

        if(std::system(nm_cmd_bigrepair.c_str()) < 0 )
        {
            std::cout<<"Error we can't exe bigrepair on files\n";
            std::cout<<"Error:"<<file<<std::endl;
            sleep(10);
            return 0;
        }

    }

    std::fstream file_f(file,std::ios::in);
    std::fstream file_c(file+".C",std::ios::in);
    std::fstream file_r(file+".R",std::ios::in);
    file_c.seekg(0,ios_base::end);
    file_r.seekg(0,ios_base::end);
    file_f.seekg(0,ios_base::end);

    size_t C = file_c.tellg()/4;
    size_t R2 = file_c.tellg();
    R2 = R2/4 - 1;
    size_t L = file_f.tellg();

    state.counters["G"] = R2 + C;
    state.counters["g"] = R2/2;
    state.counters["C"] = C;
    state.counters["DataLen"] = L;

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

        benchmark::RegisterBenchmark("run_collection_repair",repair,line)->Unit({benchmark::kMicrosecond});

    }


    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

}

