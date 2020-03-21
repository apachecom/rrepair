//
// Created by alejandro on 30-07-19.
//

#ifndef RRPAIR_DUMMYREPAIRCOMPRESSOR_H
#define RRPAIR_DUMMYREPAIRCOMPRESSOR_H


#include <string>
#include <iostream>
#include <fstream>

class DummyRepairCompressor {


    std::string cmd_repair_compressor;
    uint max_mb_ram;
public:

    DummyRepairCompressor(){}

    DummyRepairCompressor(const std::string& c, const int& mb = -1){
        cmd_repair_compressor = c;
        max_mb_ram = mb;
    }


    ~DummyRepairCompressor(){}

    int compress(const std::string& file)
    {

        std::string command_compressing= "../"+cmd_repair_compressor + " "+ file ;
//        std::cout<<command_compressing<<std::endl;


        /* std::cout<<"FILE TO COMPRESS\n";
        std::fstream rrf(file,std::ios::in);
        while(!rrf.eof()){

                int c = 0;
                rrf.read((char*)&c,4);
                std::cout<<c<<":"<<char(c)<<std::endl;
        }
         */
        
        if(system(command_compressing.c_str()) < 0) return -1;

        return 1;

    }
};


#endif //RRPAIR_DUMMYREPAIRCOMPRESSOR_H
