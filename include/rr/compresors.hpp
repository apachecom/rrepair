//
// Created by via56 on 11-12-20.
//

#ifndef RRPAIR_COMPRESORS_HPP
#define RRPAIR_COMPRESORS_HPP

extern "C"{
#include "../../external/repair/repair.h"
};
namespace rr {


    class Compressor {

        public:
            Compressor() = default;
            virtual ~Compressor() = default;
            virtual int compress(const std::string& file) = 0;
    };


    class cmdCompressor:Compressor {
    public:
        std::string bin_file{BIN_REPAIR};
        cmdCompressor() = default;
        cmdCompressor(const std::string& _bin_file):bin_file(_bin_file){}
        ~cmdCompressor(){}
        int compress(const std::string& file)
        {

            std::string command_compressing= bin_file + " "+ file ;
#ifdef RR_DEBUG_PRINT
            std::cout<<"sh-exec:"<<command_compressing<<std::endl;
#endif
            if(system(command_compressing.c_str()) < 0) return -1;
            return 1;
        }
    };

    class RePairCompressor:Compressor{

        public:
            repair _compressor;
            RePairCompressor() = default;
            ~RePairCompressor() = default;

            int compress(const std::string& file) {
                char ** cmd = new char*[2];
                cmd[0] = nullptr;
                cmd[1] = new char[file.size()];

                cmd[1][file.size()] = '\0';
                std::copy(file.begin(),file.end(),cmd[1]);
                _compressor.run(2,cmd);
            }
    };
}

#endif //RRPAIR_COMPRESORS_HPP
