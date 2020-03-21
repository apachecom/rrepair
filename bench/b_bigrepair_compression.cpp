
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <fstream>
#include <time.h>  
#include "../BigRRePair.h"
#include "../DummyPartition.h"
#include "../DummyRepairCompressor.h"
#include "../utils.h"
#include "../CLogger.h"




#include <sys/resource.h>
#include <sys/time.h>

using namespace std;

uint32_t mem_usage(){

    rusage* str_musage;
    
    if( getrusage(RUSAGE_SELF,str_musage) == 0){

        return str_musage->ru_maxrss;

    }else{
        std::cout<<"Error memory usage"<<std::endl;
    }
}


auto b_compress  = [](benchmark::State &state,const std::string& file)
{
    /*
     * Perfome setup here
     *
     * */
    
    DummyPartition P(10,100);
    DummyRepairCompressor C(myrepair_exe);
    BigRRePair brr;



    uint32_t start_mem = mem_usage();
    size_t G = 0;
    for (auto _ : state)
    {
        /*
         * This code gets timed
         *
         * */
          G = brr.rrpair(file,C,P);
    }

    uint32_t end_mem = mem_usage();

    state.counters["memory"] = end_mem - start_mem;
    state.counters["G"] = G;
};


auto b_decompress  = [](benchmark::State &state,const std::string& file)
{
    /*
     * Perfome setup here
     *
     * */
    
    DummyPartition P(10,100);
    DummyRepairCompressor C(myrepair_exe);
    BigRRePair brr;


    uint32_t start_mem = mem_usage();
    size_t G = 0;
    for (auto _ : state)
    {
        /*
         * This code gets timed
         *
         * */
          G = brr.decompress(file);
          if(G ==0 )
            sleep(4);
    }

    uint32_t end_mem = mem_usage();

    state.counters["memory"] = end_mem - start_mem;
    state.counters["G"] = G;
};



int main (int argc, char *argv[] ){

/* if(argc < 2){
        std::cout<<"bad parameters....";
        return 0;
    }
*/

     
    benchmark::RegisterBenchmark("compress chr19x50"  ,b_compress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/chr19/chr19x50.fa.out")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("compress chr19x100" ,b_compress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/chr19/chr19x100.fa.out")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("compress chr19x250" ,b_compress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/chr19/chr19x250.fa.out")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("compress chr19x500" ,b_compress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/chr19/chr19x500.fa.out")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("compress chr19x1000",b_compress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/chr19/chr19x1000.fa.out")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("compress salx815"   ,b_compress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/sal/salx815.fa.out")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("compress salx2073"  ,b_compress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/sal/salx2073.fa.out")->Unit({benchmark::kMicrosecond});
    /*
    benchmark::RegisterBenchmark("compress salx4570"  ,b_compress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/sal/salx4570.fa.out")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("compress salx11264" ,b_compress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/sal/salx11264.fa.out")->Unit({benchmark::kMicrosecond});
    */  
    
    benchmark::RegisterBenchmark("decompress chr19x50"  ,b_decompress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/chr19/chr19x50.fa.out")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("decompress chr19x100" ,b_decompress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/chr19/chr19x100.fa.out")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("decompress chr19x250" ,b_decompress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/chr19/chr19x250.fa.out")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("decompress chr19x500" ,b_decompress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/chr19/chr19x500.fa.out")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("decompress chr19x1000",b_decompress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/chr19/chr19x1000.fa.out")->Unit({benchmark::kMicrosecond});
    
    benchmark::RegisterBenchmark("decompress salx815"   ,b_decompress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/sal/salx815.fa.out")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("decompress salx2073"  ,b_decompress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/sal/salx2073.fa.out")->Unit({benchmark::kMicrosecond});
    /*benchmark::RegisterBenchmark("decompress salx4570"  ,b_decompress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/sal/salx4570.fa.out")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("decompress salx11264" ,b_decompress,"/media/alejandro/DATA/phd/pasantia/MANZZINI/sal/salx11264.fa.out")->Unit({benchmark::kMicrosecond});
    */    

//    std::string collection = "../../repetitive collections/artificial/fib41";//argv[1];
    //std::string collection = argv[1];

    /**
     * Artficial collections
    */
    /* benchmark::RegisterBenchmark("fib41",b_compress,"../../repetitiveCollections/artificial/fib41")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("rs.13",b_compress,"../../repetitiveCollections/artificial/rs.13")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("tm29",b_compress,"../../repetitiveCollections/artificial/tm29")->Unit({benchmark::kMicrosecond});
     *//**
     * Pseudo collections
    */
    /* benchmark::RegisterBenchmark("dblp.xml.00001.1",b_compress,"../../repetitiveCollections/pseudo/dblp.xml.00001.1")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("dna.001.1",b_compress,"../../repetitiveCollections/pseudo/dna.001.1")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("sources.001.2",b_compress,"../../repetitiveCollections/pseudo/sources.001.2")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("english.001.2",b_compress,"../../repetitiveCollections/pseudo/english.001.2")->Unit({benchmark::kMicrosecond});
     *//**
     * Reals collections
    */
    /* benchmark::RegisterBenchmark("coreutils",b_compress,"../../repetitiveCollections/reals/coreutils")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("Escherichia_Coli",b_compress,"../../repetitiveCollections/reals/Escherichia_Coli")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("influenza",b_compress,"../../repetitiveCollections/reals/influenza")->Unit({benchmark::kMicrosecond});
    benchmark::RegisterBenchmark("kernel",b_compress,"../../repetitiveCollections/reals/kernel")->Unit({benchmark::kMicrosecond});
 */


    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

}
