//
// Created by via56 on 11-12-20.
//

#ifndef RRPAIR_COMPRESORS_HPP
#define RRPAIR_COMPRESORS_HPP


namespace rr {


    class Compressor {

        public:
            Compressor() = default;
            virtual ~Compressor() = default;
            virtual int compress(const std::string& file) = 0;
    };


    class cmdCompressor:Compressor {
    public:
        std::string bin_file{""};
        cmdCompressor() = default;
        cmdCompressor(const std::string& _bin_file):bin_file(_bin_file){}
        ~cmdCompressor(){}
        int compress(const std::string& file)
        {
            std::string command_compressing= "../"+bin_file + " "+ file ;
            if(system(command_compressing.c_str()) < 0) return -1;
            return 1;
        }
    };
}

#endif //RRPAIR_COMPRESORS_HPP
