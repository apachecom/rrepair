
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <fstream>
#include <time.h>  
#include "../BigRRePair.h"

using namespace std;

//string filename = "../test_text_dicc";
string filename = "../../repetitive collections/pseudo/sources.001.2_dicc";


void generate_random_dicc(const uint& D, const uint& p,const uint& sigma ){
    
    srand (time(NULL));
    fstream fdicc(filename,ios::out);

    std::map<std::vector<uint>,uint> M;

    for(size_t i = 0; i < D; i++)
    {

        uint r;
        std::vector<uint> X;
        do{
            r = rand()%p;
            uint t = rand()%sigma+2;
            //fdicc.write((char*)&t,4);
            X.push_back(t);
        }while(r!=0 || M.find(X) != M.end());
        M[X] = i+1;
        uint t = 1;
        //std::cout<<std::endl;
        //fdicc.write((char*)&t,4);
    }
    uint z = 0;
    //fdicc.write((char*)&z,4);

    for(auto && ii:M){
        //std::cout<<"D"<<ii.second<<":";
        for(size_t i = 0; i < ii.first.size(); i++){
          //  std::cout<<ii.first[i]<<" ";
            uint t = ii.first[i];
            fdicc.write((char*)&t,4);
        }
        //std::cout<<"\n";
        uint t = 1;
        fdicc.write((char*)&t,4);

    }
    
    std::cout<<"END DICC GENERATION\n"<<std::endl;
}


static void test_proc_dicc(benchmark::State & state)
{

    // Perform setup here
    //generate_random_dicc(20,19,4);
     fstream fdicc(filename,ios::in);
    fstream fout(filename+"_out_t_dicc",ios::out);
    BigRRePair brr;
 
    for (auto _ : state) {
        // This code gets timed
         brr.preprocess_dicc(fdicc,fout);
//        sleep(3);
    }


}


// Register the function as a benchmark
BENCHMARK(test_proc_dicc)->Unit(benchmark::TimeUnit::kMicrosecond);

// Run the benchmark
BENCHMARK_MAIN();