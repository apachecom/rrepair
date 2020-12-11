//
// Created by via56 on 11-12-20.
//

#ifndef RRPAIR_CONFS_HPP
#define RRPAIR_CONFS_HPP

#include <vector>
#include "hash_parser.hpp"
#include "compresors.hpp"

namespace rr {



    struct params {
        std::string filename;
        uint32_t max_iter;
        uint32_t th_initial_seq;

        params() = default;
        params(const params&) = default;
        ~params() = default;
        params(const std::string& file,const uint32_t& mx,const uint32_t& th):filename(file),max_iter(mx),th_initial_seq(th){};
        params& operator = (const params& p) = default;
    };

    template<typename C,typename P>
    class RecConf {

        public:
            RecConf() = default;
            RecConf(const params& i):input(i){};
            virtual ~ RecConf(){};

            virtual void init() = 0;
            virtual bool stopCondition() = 0;
            virtual bool isFirstIteration() = 0;
            virtual void compressIteration() = 0;
            virtual void parseIteration() = 0;
            virtual void postprocess() = 0;

        protected:

            params input;
            C compressor;
            P parser;
    };

    class mzzConf:RecConf<cmdCompressor,mzzParserUC64> {

        public:

            mzzConf() = default;
            mzzConf(const params& i):RecConf<cmdCompressor, mzzParserUC64>(i){}
            virtual ~ mzzConf() = default;

            virtual void init();
            virtual bool stopCondition();
            virtual bool isFirstIteration();
            virtual void compressIteration();
            virtual void parseIteration();
            virtual void postprocess();


            cmdCompressor compressor;
            mzzParserUC64 parser;

            /**
              * internal data
            * */
            uint32_t iteration;
            uint64_t _size_seq;
            std::vector<std::pair<uint64_t , uint64_t >> maxSigmaIt; // store the sigma alph (max alp, max artificial add) used in each it
            std::vector<std::string> diccFiles;
            std::vector<std::string> parseFiles;
            std::vector<std::string> repairFilesR;
            std::vector<std::string> repairFilesC;
    };


}


#endif //RRPAIR_CONFS_HPP
