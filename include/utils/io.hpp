//
// Created by via56 on 11-12-20.
//

#ifndef RRPAIR_IO_HPP
#define RRPAIR_IO_HPP

#include <fstream>
#include <iostream>


namespace io {

    bool compareFiles(const std::string&, const std::string&);
    uint64_t getFileSize(std::fstream& );
}






#endif //RRPAIR_IO_HPP
