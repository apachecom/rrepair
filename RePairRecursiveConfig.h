//
// Created by ale on 02-07-20.
//

#ifndef RRPAIR_REPAIRRECURSIVECONFIG_H
#define RRPAIR_REPAIRRECURSIVECONFIG_H


#include <set>
#include <cstdint>
#include <fstream>
#include <stack>
#include <vector>
#include <iostream>
#include "HashParser.h"
#include "HashParserConfig.h"
#include "DummyRePair.h"
#include "RepairUtils.h"

namespace big_repair{


    template <
            typename alph_type,
            typename Compressor,
            typename Parser
    >
    class RePairRecursiveConfig {

    public:
        RePairRecursiveConfig() = default;
        virtual ~RePairRecursiveConfig() = default;
        virtual void compressor() = 0;
        virtual void parseIt() = 0;
        virtual void postprocess() = 0;
        bool stopCondition() = 0;
    };



    template <>
    class RePairRecursiveConfig <
            uint32_t,
            DummyRepair,
            HashParser< HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint64_t ,std::string > > >
            >
    {

        public:
            RePairRecursiveConfig(){
                _iter = 0;
                _compressor = nullptr;
                _parser = nullptr;
            };
            virtual ~RePairRecursiveConfig() {
                cleanTmpFiles();
            }


        protected:
            std::string _filename;
            uint32_t  _iter;
            uint32_t  _max_iter;
            uint32_t  _th_initial_seq;
            uint32_t  _size_seq;

            std::vector<std::string> diccFiles;
            std::vector<std::string> parseFiles;
            std::vector<std::string> repairFiles;

    //        uint32_t symbols;
    //        uint32_t  bytes_per_symbol;
    //
    //        std::fstream fc_out;
    //        std::fstream fr_out;
    //        std::set<alph_type> map_sigma;


            DummyRepair* _compressor;

            HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint64_t ,std::string > > _parser_conf;
        HashParser<HashParserConfig<KRPSlindingWindow<>,KRPHashFunction< uint64_t ,std::string >>>* _parser;
            std::vector<std::string> tmp_files;

            void cleanTmpFiles(){
                for(std::string s:tmp_files){

                    /*Remove all tmp file of dicc**/
                    std::string rm_cmd_old_dicc = "rm "+ s;
                    if(system(rm_cmd_old_dicc.c_str()) < 0  ){
                        std::cout<<"[ERROR] Can't remove tmp file: "+ s;
                    }

                }
                tmp_files.clear();
            }





            std::string file() const { return _filename;}

            uint32_t setIter(const uint32_t& i){ _iter = i; return _iter;}
            uint32_t iter() const { return _iter;}
            uint32_t increaseIter(){ ++_iter; return _iter;}
            uint32_t thInitialSeq() const { return _th_initial_seq;}
            uint32_t sizeSeq() const { return _size_seq;}
            uint32_t setSizeSeq( const uint32_t& s) { _size_seq = s; return _size_seq;}
            void updateParserConf(){}
            void initParserConf(){
                parseFiles.push_back(_filename);

            }


    public:

        bool stopCondition(){ return ( _iter > _max_iter ) || ( _size_seq < _max_iter ); }
        bool firstIteration(){ return !_iter; }

        /**
       * Apply repair to all files
       * */
        virtual void compressor() {
            //apply repair and store repair files
            for(std::string s:diccFiles){
                if(_compressor->apply(s) > 0){
                    repairFiles.push_back(s + ".R");
                    repairFiles.push_back(s + ".C");
                }
                else
                    throw "[ERROR] RePair compression fail";

            }
        };
        /**
        * partitioner method must create two files file_dicc and file_parse as integers
        * and Preprocess  dicc file in order to be able apply repair
        * */

        virtual void parseIt()
        {
            if(firstIteration()){
                //todo init parser conf
                initParserConf();
            }else{
                //todo init parser conf
                updateParserConf();
            }
            //parse current file
            _parser->parseFileSM();
            //posprocess dictionary
            util::prepareDiccFileForRP(
                    _parser_conf.inputFile() + ".dicc",
                    _parser_conf.inputFile()+"["+std::to_string(_iter)+"].dicc",
                    sizeof(uint32_t),
                    _parser->results._max_alph_val
                    );

            //store the dicc file for later compression
            diccFiles.push_back(_parser_conf.inputFile()+"["+std::to_string(_iter)+"].dicc");
            //store the parse file, this file will be the next to parse
            //this files will be removed in the destructor
            parseFiles.push_back(_parser_conf.inputFile() + ".parse");

            increaseIter();
        }
        /**
         *  Postprocess the files in case that it will be necesary
         * */
        virtual void postprocess() = 0;


    };

}


#endif //RRPAIR_REPAIRRECURSIVECONFIG_H
