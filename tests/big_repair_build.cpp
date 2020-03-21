//
// Created by alpachec on 18-07-19.
//


#include <iostream>
#include <gflags/gflags.h>
#include <benchmark/benchmark.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../utils/utils.h"
#include <chrono>


using namespace std::chrono;
using timer = std::chrono::high_resolution_clock;

auto big_repair = [](benchmark::State &st, const std::string& filename, const std::string& parser, const std::string& compressor,
                        const std::string& procdic = "../bin/procdic",const std::string& postprocdic = "../bin/postproc",
                        const int & max_mb_ram = 0, const int & mod = 100, const int & size_windows = 10,
                        const bool & verbose = true, const bool &keep_files = false, const bool &full_report = true ){
    //uses newscanNT
    std::string command_parsing = parser + " " +filename + " -w " + std::to_string(size_windows) + " -p " + std::to_string(mod) +" -c "+ (verbose?" -v ":" ");
    std::string command_preproc_dicc = procdic + " "+filename+".dicz";
    std::string command_compressing_dicc = compressor + " "+ filename +".dicz.int " + (( max_mb_ram > 0 )? std::to_string(max_mb_ram):" " );
    std::string command_compressing_parse = compressor + " "+ filename +".parse " + (( max_mb_ram > 0 )? std::to_string(max_mb_ram):" " );;
    std::string command_postprocessing_dicc = postprocdic + " " + filename;

    int64_t time_parsing = 0,time_preproc_dicc = 0,time_compressing_dicc = 0,time_compressing_parse = 0, time_postprocessing_dic = 0;
    int64_t it = 0;

    double vm, rss;
    process_mem_usage(vm,rss);
    st.counters["rss"] = rss;
    st.counters["vm"] = vm;

    for (auto _ : st)
    {
        std::cout<<command_parsing<<std::endl;
        auto start = timer::now();
        if(system(command_parsing.c_str()) < 0 ){
            std::cout<<"Error in "+command_parsing<<std::endl;
            sleep(3);
            return;
        }
        auto stop = timer::now();
        time_parsing += duration_cast<milliseconds>(stop-start).count();

        if(!keep_files)
        {
            std::string command_rem_tmp_files = "rm -f "+filename+".parse_old "+filename+".last "+filename+".occ";
            std::cout<<command_rem_tmp_files<<std::endl;
            if(system(command_rem_tmp_files.c_str()) < 0){
                std::cout<<"Error in "+command_rem_tmp_files<<std::endl;
                sleep(3);
                return;
            }
        }


        std::cout<<command_preproc_dicc<<std::endl;
        start = timer::now();
        if(system(command_preproc_dicc.c_str()) < 0){
            std::cout<<"Error in "+command_preproc_dicc<<std::endl;
            sleep(3);
            return;
        }
        stop = timer::now();
        time_preproc_dicc += duration_cast<milliseconds>(stop-start).count();


        std::cout<<command_compressing_dicc<<std::endl;
        start = timer::now();
        if(system(command_compressing_dicc.c_str()) < 0){
            std::cout<<"Error in "+command_compressing_dicc<<std::endl;
            sleep(3);
            return;
        }
        stop = timer::now();
        time_compressing_dicc += duration_cast<milliseconds>(stop-start).count();


        std::cout<<command_compressing_parse<<std::endl;
        start = timer::now();
        if(system(command_compressing_parse.c_str()) < 0){
            std::cout<<"Error in "+command_compressing_parse<<std::endl;
            sleep(3);
            return;
        }
        stop = timer::now();
        time_compressing_parse += duration_cast<milliseconds>(stop-start).count();


        std::cout<<command_postprocessing_dicc<<std::endl;
        start = timer::now();
        if(system(command_postprocessing_dicc.c_str()) < 0){
            std::cout<<"Error in "+command_postprocessing_dicc<<std::endl;
            sleep(3);
            return;
        }
        stop  = timer::now();
        time_postprocessing_dic += duration_cast<milliseconds>(stop-start).count();

        if(!keep_files)
        {

            std::string command_rem_tmp_files = "rm -f "+filename+".parse "+filename+".parse.C "+filename+".parse.R "+filename+".dicz "+filename+".dicz.int "+filename+".dicz.int.C "+filename+".dicz.int.R";
            std::cout<<command_rem_tmp_files<<std::endl;
            if(system(command_rem_tmp_files.c_str()) < 0) {
                std::cout<<"Error in "+command_rem_tmp_files<<std::endl;
                sleep(3);
                return;
            }
        }

        ++it;
    }



//    st.counters["parsing_time"]               = time_parsing*1.0/it;
//    st.counters["preprocessing_parse_time"]   = time_preproc_dicc*1.0/it;
//    st.counters["compressing_dicc_time"]      = time_compressing_dicc*1.0/it;
//    st.counters["compressing_parse_time"]     = time_compressing_parse*1.0/it;
//    st.counters["postprocessing_dicc_time"]   = time_postprocessing_dic*1.0/it;


    std::cout<<"parsing_time"<< time_parsing*1.0/it <<std::endl;
    std::cout<<"preprocessing_parse_time"<< time_preproc_dicc*1.0/it <<std::endl;
    std::cout<<"compressing_dicc_time"<< time_compressing_dicc*1.0/it <<std::endl;
    std::cout<<"compressing_parse_time"<< time_compressing_parse*1.0/it <<std::endl;
    std::cout<<"postprocessing_dicc_time"<< time_postprocessing_dic*1.0/it <<std::endl;


};

auto solca = [](benchmark::State &st, const std::string& filename, const std::string& output){

    std::string cmd ="../SOLCA/solca/src/compress -i " + filename + " -o " + output + "\n";

    double vm, rss;
    process_mem_usage(vm,rss);
    st.counters["rss"] = rss;
    st.counters["vm"] = vm;

    for (auto _ : st)
    {
        std::cout<<cmd<<std::endl;
        if(system(cmd.c_str()) < 0) return;
    }



};


void help(){

    printf("Arguments:\n");
    printf("<filename>\n");
    printf("<parser>\n");
    printf("<compressor>\n");
    printf("<procdic>\n");
    printf("<postprocdic>\n");
    printf("<max_mb_ram>\n");
    printf("<mod>\n");
    printf("<size_windows>\n");
    printf("<verbose>\n");
    printf("<keep_files>\n");
    printf("<full_report>\n");
}




int main (int argc, char *argv[] )
{

    if(argc < 2){
        std::cout<<"bad parameters....";
        std::cout<<"you need to pass de <filename>";
    }
    std::string filename = (argv[1]);
//    std::string filename = "../test.txt";

    benchmark::RegisterBenchmark("big-repair", big_repair,filename,"../bin/newscanNT.x","../bin/repair")->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("solca", solca,filename,filename+".c")->Unit(benchmark::kMillisecond);


    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

}