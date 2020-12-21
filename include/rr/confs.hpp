//
// Created by via56 on 11-12-20.
//

#ifndef RRPAIR_CONFS_HPP
#define RRPAIR_CONFS_HPP

#include <vector>
#include "hash_parser.hpp"
#include "compresors.hpp"
#include "macros.hpp"
namespace rr {



    struct params {
        std::string filename;
        uint32_t max_iter;
        uint32_t w;
        uint32_t m;
        uint32_t th_initial_seq;

        params() = default;
        params(const params&) = default;
        ~params() = default;
        params(const std::string& file,const uint32_t& mx,const uint32_t& th, const uint32_t&_w, const uint32_t&_m):filename(file),max_iter(mx)
        ,th_initial_seq(th),w(_w),m(_m){};
        params& operator = (const params& p) = default;
    };

    template<typename C,typename P>
    class RecConf {

        public:
            RecConf() = default;
            RecConf(const params& i):input(i){};
            virtual ~ RecConf(){};

            virtual void init(const params *) = 0;
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

    class mzzConf:public RecConf<RePairCompressor,mzzParserUC64> {

        public:

            mzzConf() = default;
            mzzConf(const mzzConf& mC){
                this->input = mC.input;
            }
            mzzConf& operator=(const mzzConf& mC){

                input = mC.input;
                parser = mC.parser;
                target_file = mC.target_file;
                parseFiles = mC.parseFiles;
                diccFiles = mC.diccFiles;
                iteration = mC.iteration;
                _size_seq = mC._size_seq;
                maxSigmaIt = mC.maxSigmaIt;
                repairFilesR = mC.repairFilesR;
                repairFilesC = mC.repairFilesC;


                return *this;
            }
            mzzConf(const params& i):RecConf<RePairCompressor, mzzParserUC64>(i){
                }
            virtual ~ mzzConf() = default;

            virtual void init(const params *);
            virtual bool stopCondition();
            virtual bool isFirstIteration();
            virtual void compressIteration();
            virtual void parseIteration();
            virtual void postprocess();


            RePairCompressor compressor;
            mzzParserUC64 parser;

            /**
              * internal data
            * */
            uint32_t iteration{0};
            std::string target_file{""};
            uint64_t _size_seq{std::numeric_limits<uint64_t>::max()};
            std::vector<std::pair<uint64_t , uint64_t >> maxSigmaIt; // store the sigma alph (max alp, max artificial add) used in each it
            std::vector<std::string> diccFiles;
            std::vector<std::string> parseFiles;
            std::vector<std::string> repairFilesR;
            std::vector<std::string> repairFilesC;


        protected:


        uint64_t postprocessDiccRFile(std::fstream &file, std::fstream &R, const uint64_t  &offset_rules, const uint64_t  &min_sigma, std::map<uint64_t , uint64_t > &rule_map);

        void postprocessDiccCFile(std::fstream &file, std::fstream &R, uint64_t &n_rules_level, const uint64_t  &offset_rules,const uint64_t  &min_sigma,const uint64_t &max_sigma,std::map<uint64_t , uint64_t > &rule_map);

        std::pair<uint64_t , uint64_t > posprocessingFinalParseFile(std::fstream &R,std::fstream &C,uint64_t &offset_rules,const uint64_t &initial_sigma,std::map<uint64_t , uint64_t > &rule_map);
    };


}


#endif //RRPAIR_CONFS_HPP
