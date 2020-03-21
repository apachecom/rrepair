//
// Created by alejandro on 30-07-19.
//


#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <fstream>
#include <time.h>  
#include "../utils.h"
#include "../DummyPartition.h"
#define BYTES 1


DummyPartition P(10,100,parseNT_exe);

using namespace std;
//string filename = "/media/alejandro/DATA1/phd/pasantia/MANZZINI/chr19/chr19x50.fa.out";
//string filename = "/media/alejandro/DATA1/phd/pasantia/MANZZINI/sal/salx4570.fa.out";
string filename = "/media/alejandro/DATA1/phd/pasantia/MANZZINI/sal/salx11264.fa.out";
//string filename = "/media/alejandro/DATA1/phd/pasantia/repetitiveCollections/reals/influenza";
//string filename = "../test_text";

static void test_partition_bigrepairmzz(benchmark::State & state)
{

    // Perform setup here

    for (auto _ : state) {

        std::string cmp_cmd = "../repos/bigrepairmzz/bigrepair/ctph/newscan.x "+filename+" ";


        if(std::system(cmp_cmd.c_str()) < 0) {
            std::cout<<"Unable to execute :"<<cmp_cmd<<std::endl;
            sleep(5);
            return;
        }
   /*      std::cout<<"showing phrases:\n";

        fstream fdict(filename+".dict",std::ios::in);
        char c;
        std::string s = "";
        std::map<uint,std::string> M;
        uint i = 0,wc = 0,pl = 0;

        while(!fdict.eof() && fdict.read((char*)&c,1)){
            if(c == 0 || c == -1) break;
            if(c == 1){
                M[++i] = s;
                std::cout<<s<<std::endl;
                s = "";
                ++wc;
            }
            s += c;
            
        }


        fstream fparse(filename+".parse",std::ios::in);
        fstream fd(filename+".dc",std::ios::out);
        std::cout<<"showing parse:\n";
        
        size_t len = 0;
        
        uint pp;
        
        while(!fparse.eof() && fparse.read((char*)&pp,4)){
            
            if(M.find(pp) == M.end()){
                std::cout<<"ERROR IN DICC\n";
                sleep(10);
            }
            std::string ss = M[pp];
            
            len+=ss.size();
            
            fd.write(ss.c_str(),ss.size());++pl;
            
            std::cout<<pp<<"->"<<ss<<std::endl;

        }

        std::cout<<"wc "<<wc<<std::endl;
        std::cout<<"pl "<<pl<<std::endl;
        std::cout<<"len "<<len<<std::endl;
        sleep(5);   */
    }


}
static void test_partition_bigrepair(benchmark::State & state)
{

    // Perform setup here

    for (auto _ : state) {

        std::string cmp_cmd = "../repos/bigrepair/ctph/newscan.x "+filename+" ";


        if(std::system(cmp_cmd.c_str()) < 0) {
            std::cout<<"Unable to execute :"<<cmp_cmd<<std::endl;
            sleep(5);
            return;
        }
         std::cout<<"showing phrases:\n";

        fstream fdict(filename+".dict",std::ios::in);
        char c;
        std::string s = "";
        std::map<uint,std::string> M;
        uint i = 0,wc = 0,pl = 0;

        while(!fdict.eof() && fdict.read((char*)&c,1)){
            if(c == 0 || c == -1) break;
            if(c == 1){
                M[++i] = s;
//                std::cout<<s<<std::endl;
                s = "";
                ++wc;
            }
            s += c;
            
        }


        fstream fparse(filename+".parse",std::ios::in);
        fstream fd(filename+".dc",std::ios::out);
        std::cout<<"showing parse:\n";
        
        size_t len = 0;
        
        uint pp;
        
        while(!fparse.eof() && fparse.read((char*)&pp,4)){
            
            if(M.find(pp) == M.end()){
                std::cout<<"ERROR IN DICC\n";
                sleep(10);
            }
            std::string ss = M[pp];
            
            len+=ss.size();
            
            fd.write(ss.c_str(),ss.size());++pl;
            
//            std::cout<<pp<<"->"<<ss<<std::endl;

        }

        std::cout<<"wc "<<wc<<std::endl;
        std::cout<<"pl "<<pl<<std::endl;
        std::cout<<"len "<<len<<std::endl;
        sleep(5);
    }


}
static void test_partition_bigrepairNT(benchmark::State & state)
{

    // Perform setup here

    for (auto _ : state) {

        std::string cmp_cmd = "../repos/bigrepair/ctph/newscanNT.x "+filename+" ";


        if(std::system(cmp_cmd.c_str()) < 0) {
            std::cout<<"Unable to execute :"<<cmp_cmd<<std::endl;
            sleep(5);
            return;
        }
/* 
        std::cout<<"showing phrases:\n";

        fstream fdict(filename+".dict",std::ios::in);
        char c;
        std::string s = "";
        std::map<uint,std::string> M;
        uint i = 0,wc = 0,pl = 0;

        while(!fdict.eof() && fdict.read((char*)&c,1)){
            if(c <= 1){
                M[++i] = s;
                std::cout<<s<<std::endl;
                s = "";
                ++wc;
            }
            else{
                if(c == 2){
                    std::cout<<"CARACTER DOLAR ENCONTRADO\n";
                }
                else s += c;
            }
        }


        fstream fparse(filename+".parse",std::ios::in);
        fstream fd(filename+".dc",std::ios::out);
        std::cout<<"showing parse:\n";
        
        size_t len = 0;
        
        uint pp;
        
        while(!fparse.eof() && fparse.read((char*)&pp,4)){
            
            if(M.find(pp) == M.end()){
                std::cout<<"ERROR IN DICC\n";
                sleep(10);
            }
            std::string ss = M[pp];
            
            len+=ss.size();
            
            fd.write(ss.c_str(),ss.size());++pl;
            
            std::cout<<pp<<"->"<<ss<<std::endl;

        }

        std::cout<<"wc "<<wc<<std::endl;
        std::cout<<"pl "<<pl<<std::endl;
        std::cout<<"len "<<len<<std::endl;
        sleep(5);   */
    }


}

static void test_partition(benchmark::State & state)
{
    // Perform setup here       

    for (auto _ : state) {

         // This code gets timed

        /* {
            fstream ff(filename,std::ios::in);
            fstream ffo(filename+".BYTES",std::ios::out);
            int v = 0;
            
            while(!ff.eof() && ff.read((char*)&v,BYTES))
            {
                ffo.write((char*)&v,BYTES);
                v = 0;
            }
        } */

//        P.parse_file(filename,BYTES);
        P.parse_byte_fileb(filename);
//        fstream fout_dicc(filename+"_dicc",ios::in);
//        fstream fout_parse(filename+"_parse",ios::in);
//        std::unordered_map<uint, std::string> D;
//
//        uint d = 0;
//        std::string word= "" ;
//        uint c = 0;
//
//        if(!fout_dicc.is_open())
//           std::cout<<"Error opening dicc file\n";
//
//        while(!fout_dicc.eof() && fout_dicc.read((char*)&c,4)){
//
//           if(c == 0){
//               D[++d] = word;
//               if(d < 10) std::cout<<d<<":"<<word<<std::endl;
//        		word.clear();
//
//            }
//            else{
//                word.push_back((char)c);
//            }
//        }
//
//        fstream file2(filename+"_dp",ios::out);
//
//        int c2;
//        std::cout<<"size dicc:"<<D.size()<<std::endl;
//
//        if(!fout_parse.is_open())
//            std::cout<<"Error opening parser file\n";
//        int i = 0;
//        while(!fout_parse.eof() && fout_parse.read((char*)&c2,4)){
//
//                if(++i < 10)std::cout<<c2<<" ";
//                file2.write(D[c2].c_str(),D[c2].size());
//
//        }


//        std::string cmp_cmd = "cmp "+filename+" "+filename+"_dp";
//
//       std::cout<<"Unable to execute :"<<cmp_cmd<<std::endl;
//
//
//        if(std::system(cmp_cmd.c_str()) < 0) ;
    }
}

static void test_partition_int(benchmark::State & state)
{
    // Perform setup here

    for (auto _ : state) {

        // This code gets timed

        /* {
            fstream ff(filename,std::ios::in);
            fstream ffo(filename+".BYTES",std::ios::out);
            int v = 0;

            while(!ff.eof() && ff.read((char*)&v,BYTES))
            {
                ffo.write((char*)&v,BYTES);
                v = 0;
            }
        } */

        P.parse_fileb(filename,BYTES);
//        P.parse_byte_file(filename);
//
//        fstream fout_dicc(filename+"_dicc",ios::in);
//        fstream fout_parse(filename+"_parse",ios::in);
//        std::unordered_map<uint, std::vector<uint>> D;
//
//        uint d = 0;
//        std::vector<uint> word ;
//        uint c = 0;
//
//        if(!fout_dicc.is_open())
//            std::cout<<"Error opening dicc file\n";
//
//        while(!fout_dicc.eof() && fout_dicc.read((char*)&c,4)){
//
//            if(c == 0){
//                D[++d] = word;
//                word.clear();
//            }
//            else{
//                word.push_back((uint)c);
//            }
//        }
//
//        fstream file2(filename+"_dp_int",ios::out);
//
//        int c2;
//        std::cout<<"size dicc:"<<D.size()<<std::endl;
//
//        if(!fout_parse.is_open())
//            std::cout<<"Error opening parser file\n";
//
//        while(!fout_parse.eof() && fout_parse.read((char*)&c2,4)){
//
//            for(auto&& ii : D[c2]){
//
//                uint v = ii;
//                file2.write((char*)&v,BYTES);
//            }
//        }
//
//
//        std::string cmp_cmd = "cmp "+filename+" "+filename+"_dp";
//
//        if(std::system(cmp_cmd.c_str()) < 0) ;
//        std::cout<<"Unable to execute :"<<cmp_cmd<<std::endl;

    }
}


static void test_partition_dumm(benchmark::State & state)
{
    // Perform setup here

    for (auto _ : state) {

        // This code gets timed

        /* {
            fstream ff(filename,std::ios::in);
            fstream ffo(filename+".BYTES",std::ios::out);
            int v = 0;

            while(!ff.eof() && ff.read((char*)&v,BYTES))
            {
                ffo.write((char*)&v,BYTES);
                v = 0;
            }
        } */

        P.partition(filename,BYTES);
//        P.parse_byte_file(filename);
//
//        fstream fout_dicc(filename+"_dicc",ios::in);
//        fstream fout_parse(filename+"_parse",ios::in);
//        std::unordered_map<uint, std::vector<uint>> D;
//
//        uint d = 0;
//        std::vector<uint> word ;
//        uint c = 0;
//
//        if(!fout_dicc.is_open())
//            std::cout<<"Error opening dicc file\n";
//
//        while(!fout_dicc.eof() && fout_dicc.read((char*)&c,4)){
//
//            if(c == 0){
//                D[++d] = word;
//                word.clear();
//            }
//            else{
//                word.push_back((uint)c);
//            }
//        }
//
//        fstream file2(filename+"_dp_int",ios::out);
//
//        int c2;
//        std::cout<<"size dicc:"<<D.size()<<std::endl;
//
//        if(!fout_parse.is_open())
//            std::cout<<"Error opening parser file\n";
//
//        while(!fout_parse.eof() && fout_parse.read((char*)&c2,4)){
//
//            for(auto&& ii : D[c2]){
//
//                uint v = ii;
//                file2.write((char*)&v,BYTES);
//            }
//        }
//
//
//        std::string cmp_cmd = "cmp "+filename+" "+filename+"_dp";
//
//        if(std::system(cmp_cmd.c_str()) < 0) ;
//        std::cout<<"Unable to execute :"<<cmp_cmd<<std::endl;

    }
}

// Register the function as a benchmark

//BENCHMARK(test_partition_bigrepair)->Unit(benchmark::TimeUnit::kMicrosecond);
BENCHMARK(test_partition_bigrepairNT)->Unit(benchmark::TimeUnit::kMicrosecond);
//BENCHMARK(test_partition_bigrepairmzz)->Unit(benchmark::TimeUnit::kMicrosecond);
//BENCHMARK(test_partition_dumm)->Unit(benchmark::TimeUnit::kMicrosecond);
//BENCHMARK(test_partition)->Unit(benchmark::TimeUnit::kMicrosecond);
//BENCHMARK(test_partition_int)->Unit(benchmark::TimeUnit::kMicrosecond);
// Run the benchmark
BENCHMARK_MAIN();
