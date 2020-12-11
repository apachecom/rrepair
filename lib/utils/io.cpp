//
// Created by via56 on 11-12-20.
//

#include "../../include/utils/io.hpp"

using namespace io;

bool io::compareFiles(const std::string& p1, const std::string& p2) {

    std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
    std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

    if (f1.fail() || f2.fail()) {
        std::cout<<"f1.fail() || f2.fail()"<<std::endl;
        return false; //file problem
    }

    if (f1.tellg() != f2.tellg()) {
        std::cout<<"f1.tellg() != f2.tellg() "<<f1.tellg()<<" "<<f2.tellg()<<std::endl;
        return false; //size mismatch
    }

    //seek back to beginning and use std::equal to compare contents
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f2.rdbuf()));
}

uint64_t io::getFileSize(std::fstream& file){
    std::streampos fsize;
    fsize = file.tellg();
    file.seekg( 0, std::ios::end );
    fsize = file.tellg() - fsize;
    file.seekg( 0, std::ios::beg );
    file.clear();
    return (uint64_t) fsize;
}