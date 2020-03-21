//
// Created by alejandro on 30-07-19.
//


#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <fstream>
#include <functional>
#include <map>
#include <unordered_map>
#include <time.h>  
#include "../utils.h"
#include "../DummyRepairCompressor.h"

std::string irepair = "bin/irepair";
DummyRepairCompressor C(myrepair_exe);

using namespace std;

//string filename = "../../repetitiveCollections/pseudo/dna.001.1";
//string filename = "../../repetitiveCollections/pseudo/sources.001.2";
string filename = "../test_text";


static void test_compressor(benchmark::State & state)
{

    // Perform setup here    
    
    for (auto _ : state) {
         // This code gets timed
         std::fstream cf(filename,std::ios::in);
         std::fstream cfint(filename+".int",std::ios::out);
         
         int c;
         while(!cf.eof() && cf.read((char*)&c,1)){
             cfint.write((char*)&c,sizeof(int));
         }

         C.compress(filename+".int");
    }
}


static void test_decompressor(benchmark::State & state)
{

    // Perform setup here    
    std::fstream fr(filename+".int.R",std::ios::in);
    std::fstream fc(filename+".int.C",std::ios::in);
    std::fstream fd(filename+".int.dec",std::ios::out);
    std::fstream cf(filename+".int",std::ios::in);
    std::map<uint,uint> S;
    
    std::cout<<"Original sigma\n";
    {
        uint t;
        uint iii = 0;
        while(!cf.eof() && cf.read((char*)&t,4))
            if(S.find(t) == S.end())
                S[t] = t;
    }

    std::cout<<S.size()<<std::endl;
    
    sleep(5);       


    for (auto _ : state) {
         // This code gets timed

        int sigma = 0;
        
        fr.read((char*)&sigma,sizeof(int));
        
        std::cout<<"repair sigma:" << sigma << std::endl;
        
        sleep(3);     
        
        std::unordered_map<int,std::pair<int,int>> R;

        int X,Y;

        int r = sigma;
        
        bool fir = 0;
        
        while(!fr.eof() && fr.read((char*)&X,sizeof(int)) && fr.read((char*)&Y,sizeof(int)))
        {
            R[r] = make_pair(X,Y);
            r++;
        }
        std::cout<<"number of rules:"<<r<<std::endl;
        sleep(5);
        std::function< void (const int& , std::fstream & ) > f;
        
        f = [&sigma, &R, &f](const int& i, std::fstream& out ){
                //std::cout<<i<<std::endl;    
                if(i < sigma){
                    out.write((char*)&i,1);
                  //  std::cout<<S[i]<<std::endl;
                    return;
                }

                if(R.find(i) == R.end()){
                    std::cout<<"Error in the files\n";
                    return;
                }
                f(R[i].first,out);
                f(R[i].second,out);                    
        };

        int C = 0;
        
        while(!fc.eof() && fc.read((char*)&C,sizeof(int))){
            f(C,fd);
        }
    }
}


// Register the function as a benchmark
BENCHMARK(test_compressor)->Unit(benchmark::TimeUnit::kMicrosecond);
BENCHMARK(test_decompressor)->Unit(benchmark::TimeUnit::kMicrosecond);
// Run the benchmark
BENCHMARK_MAIN();