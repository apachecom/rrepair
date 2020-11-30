//
// Created by via56 on 29-11-20.
//


#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include <sys/stat.h>
#include <fstream>

extern "C" {
#include "../../include/_c/utils.h"
}





auto firstTest = [](benchmark::State & state)
{
    // Perform setup here


    for (auto _ : state) {
        // This code gets timed
    }

};
auto writeFstream = [](benchmark::State & state,const std::string &file, int numBytes){
    // Perform setup here

    for (auto _ : state) {
        // This code gets timed
        char* buffer = new char[numBytes];
        for (int i = 0; i < numBytes; ++i) {
            char c = rand()%255;
            buffer[i] = c;
        }

        std::fstream f(file,std::ios::in);
        if(!f.is_open()){
            fprintf (stderr,"Error: cannot read file %s\n",file.c_str());
            exit(1);
        }
        f.write(buffer,numBytes);

        delete buffer;
    }

};
auto readFstream = [](benchmark::State & state,const std::string &file)
{
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
        struct stat s;
        if (stat (file.c_str(),&s) != 0){
            fprintf (stderr,"Error: cannot stat file %s\n",file.c_str());
            exit(1);
        }
//        uint64_t len = s.st_size/sizeof(char);
        char* buffer = new char[10];
        std::fstream f(file,std::ios::in);
        if(!f.is_open()){
            fprintf (stderr,"Error: cannot read file %s\n",file.c_str());
            exit(1);
        }
        f.read(buffer,10*sizeof(char));
        delete buffer;
    }

};


auto writeFILE = [](benchmark::State & state,const std::string &file,int numBytes)
{
    // Perform setup here
    srand(time(nullptr));


    for (auto _ : state) {
        char* buffer = (char*) malloc(numBytes*sizeof(char));
        for (int i = 0; i < numBytes; ++i) {
            char c = rand()%255;
            buffer[i] = c;
        }

        // This code gets timed
        FILE *f = fopen(file.c_str(),"rb");
        if(f==NULL)
            die(file.c_str());
        fwrite(&buffer,sizeof(char ),numBytes,f);

        fclose(f);
        free(buffer);
    }
};

auto readFILE = [](benchmark::State & state,const std::string &file)
{
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
        struct stat s;
        if (stat (file.c_str(),&s) != 0)
        {
            fprintf (stderr,"Error: cannot stat file %s\n",file.c_str());
            exit(1);
        }
        uint64_t len = s.st_size/sizeof(char);

        FILE *f = fopen(file.c_str(),"rb");

        if(f==NULL)
            die(file.c_str());

        char* buffer = (char*) malloc(10*sizeof(char));

        if (fread(buffer,sizeof(char),10,f) != len) {
            fprintf (stderr,"Error: cannot read file %s\n",file.c_str());
            fclose(f);
            exit(1);
        }
        fclose(f);
        free(buffer);

    }

};

int main (int argc, char *argv[] ){

        std::string s = argv[1]; //collection

        benchmark::RegisterBenchmark("start"  ,firstTest)->Unit({benchmark::kMicrosecond});
        benchmark::RegisterBenchmark("read-FILE"  ,readFILE,s)->Unit({benchmark::kMicrosecond});
        benchmark::RegisterBenchmark("read-FSTREAM"  ,readFstream,s)->Unit({benchmark::kMicrosecond});
//
        benchmark::RegisterBenchmark("write-FILE"  ,writeFILE,"tmp-out",1)->Unit({benchmark::kMicrosecond});
        benchmark::RegisterBenchmark("write-FSTREAM"  ,writeFstream,"tmp-out",1)->Unit({benchmark::kMicrosecond});


    benchmark::Initialize(&argc, argv);
        benchmark::RunSpecifiedBenchmarks();

    return 0;

}