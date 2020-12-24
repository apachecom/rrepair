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
#include <limits>
#include "HashParser.h"
#include "HashParserConfig.h"
#include "DummyRePair.h"
#include "RepairUtils.h"
#include "randomPermutationParser.hpp"
#include "macros.h"

#include <chrono>

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

        typedef HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint64_t ,std::string > > ParseConf;
        typedef HashParser<HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint64_t ,std::string > > > Parse;
//        const uint32_t& ws,const uint32_t& b,const uint32_t& m,const std::string& input, const std::string& tempDir

        public:
            RePairRecursiveConfig(){
                _iter = 0;
                _parser = nullptr;
            };


            RePairRecursiveConfig(const ParseConf& pc,const DummyRepair& c, const uint32_t& max_iter,const uint32_t& th_initial_seq){

                _iter = 0;
                _compressor = c;  //dummy compressor
                _filename = pc.inputFile();
                _parser_conf = pc; // paser conf
                _parser = nullptr; // pointer to the current parser
                _max_iter = max_iter; // max number of iterations
                _th_initial_seq = th_initial_seq; // threshold for sequence len
                _size_seq = std::numeric_limits<uint32_t>::max();

            };
            RePairRecursiveConfig(const RePairRecursiveConfig <
                    uint32_t,
                    DummyRepair,
                    HashParser< HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint64_t ,std::string > > >
            >& RPC){

                /**
                 * Parameters
                 * */

                _filename = RPC._filename;
                _iter = RPC._iter;
                _max_iter = RPC._max_iter;
                _th_initial_seq = RPC._th_initial_seq;
                _compressor = RPC._compressor;
                _parser = nullptr;
                _parser_conf = RPC._parser_conf;
                _size_seq = RPC._size_seq;

            }
            RePairRecursiveConfig& operator=(const RePairRecursiveConfig <
                    uint32_t,
                    DummyRepair,
                    HashParser< HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint64_t ,std::string > > >
            >& RPC){

                _filename = RPC._filename;
                _iter = RPC._iter;
                _max_iter = RPC._max_iter;
                _th_initial_seq = RPC._th_initial_seq;
                _compressor = RPC._compressor;
                _parser = nullptr;
                _parser_conf = RPC._parser_conf;
                _size_seq = RPC._size_seq;
                return *this;
            }
            virtual ~RePairRecursiveConfig() {
                cleanTmpFiles();
                if(_parser != nullptr) delete _parser;
            }


        protected:
            /**
             * Parameters
             * */
            std::string _filename;
            uint32_t  _iter;
            uint32_t  _max_iter;
            uint32_t  _th_initial_seq;

            /**
             * internal data
             * */
            uint32_t  _size_seq;
            std::vector<std::pair<uint32_t,uint32_t>> maxSigmaIt; // store the sigma alph (max alp, max artificial add) used in each it
            std::vector<std::string> diccFiles;
            std::vector<std::string> parseFiles;
            std::vector<std::string> repairFilesR;
            std::vector<std::string> repairFilesC;

            DummyRepair _compressor;

            ParseConf _parser_conf;
            Parse* _parser;

            void cleanTmpFiles(){
//                for(std::string s:tmp_files){
//                    /*Remove all tmp file of dicc**/
//                    std::string rm_cmd_old_dicc = "rm "+ s;
//                    if(system(rm_cmd_old_dicc.c_str()) < 0  ){
//                        std::cout<<"[ERROR] Can't remove tmp file: "+ s;
//                    }
//
//                }
//                tmp_files.clear();

                for(std::string s:diccFiles){
                    std::string rm_cmd_old_dicc = "rm "+ s;
                    if(system(rm_cmd_old_dicc.c_str()) < 0  ){
                        std::cout<<"[ERROR] Can't remove tmp file: "+ s;
                    }
                }
                for(std::string s:parseFiles){
                    std::string rm_cmd_old_dicc = "rm "+ s;
                    if(system(rm_cmd_old_dicc.c_str()) < 0  ){
                        std::cout<<"[ERROR] Can't remove tmp file: "+ s;
                    }
                }
                for(std::string s:repairFilesR){
                    std::string rm_cmd_old_dicc = "rm "+ s;
                    if(system(rm_cmd_old_dicc.c_str()) < 0  ){
                        std::cout<<"[ERROR] Can't remove tmp file: "+ s;
                    }
                }
                for(std::string s:repairFilesC){
                    std::string rm_cmd_old_dicc = "rm "+ s;
                    if(system(rm_cmd_old_dicc.c_str()) < 0  ){
                        std::cout<<"[ERROR] Can't remove tmp file: "+ s;
                    }
                }
            }

            std::string file() const { return _filename;}

            uint32_t setIter(const uint32_t& i){ _iter = i; return _iter;}
            uint32_t iter() const { return _iter;}
            uint32_t increaseIter(){ ++_iter; return _iter;}
            uint32_t thInitialSeq() const { return _th_initial_seq;}
            uint32_t sizeSeq() const { return _size_seq;}
            uint32_t setSizeSeq( const uint32_t& s) { _size_seq = s; return _size_seq;}

            void updateParserConf(){
                _parser_conf.setInputFile(parseFiles[_iter-1]);
                _parser_conf.setBytesToRead(sizeof(uint32_t));
            }
            void initParserConf(){
                _parser_conf.setInputFile(_filename);
                _parser_conf.setBytesToRead(1);

            }


    public:

        bool stopCondition(){
                return ( _iter > _max_iter ) || ( _size_seq < _th_initial_seq );
            }
        bool firstIteration(){ return !_iter; }

        /**
       * Apply repair to all files
       * */
        void compressor() {
            //apply repair and store repair files
            for(std::string s:diccFiles){

#ifdef MEASURE_TIME
                auto start = std::chrono::high_resolution_clock::now();
#endif
                if(_compressor.apply(s) > 0){
                    repairFilesR.push_back(s + ".R");
                    repairFilesC.push_back(s + ".C");
                }
                else
                    throw "[ERROR] RePair compression fail dicc file " + s;

#ifdef MEASURE_TIME
                auto end = std::chrono::high_resolution_clock::now();
                auto elapse = std::chrono::duration_cast<std::chrono::microseconds>( end - start).count();
                CLogger::GetLogger()->model[s + ":repair:time"] = elapse;
#endif

            }

#ifdef MEASURE_TIME
            auto start = std::chrono::high_resolution_clock::now();
#endif

            std::string last_parse_file = parseFiles.back();
            if(_compressor.apply(last_parse_file) > 0){
                repairFilesR.push_back(last_parse_file + ".R");
                repairFilesC.push_back(last_parse_file + ".C");
            } else throw "[ERROR] RePair compression fail last parser file";

#ifdef MEASURE_TIME
            auto end = std::chrono::high_resolution_clock::now();
            auto elapse = std::chrono::duration_cast<std::chrono::microseconds>( end - start).count();
            CLogger::GetLogger()->model[last_parse_file + ":repair:time"] = elapse;
#endif

        };
        /**
        * partitioner method must create two files file_dicc and file_parse as integers
        * and Preprocess  dicc file in order to be able apply repair
        * */

        virtual void parseIt()
        {
            if(firstIteration()){
                initParserConf();
            }else{
                updateParserConf();
            }
            //config parse;
            if(_parser != nullptr)
                delete _parser;

            _parser = new Parse( _parser_conf );
            //parse current file

#ifdef MEASURE_TIME
            {
                auto start = std::chrono::high_resolution_clock::now();

#endif
                _parser->parseFileSM();

#ifdef MEASURE_TIME
                auto end = std::chrono::high_resolution_clock::now();
                auto elapse = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                CLogger::GetLogger()->model[_parser_conf.inputFile() + ":parse:time"] = elapse;
            }
#endif

            uint32_t max_sigma = 0;
#ifdef MEASURE_TIME
            {
                auto start = std::chrono::high_resolution_clock::now();

#endif

            //posprocess dictionary
            _size_seq = _parser->results._seq_len;
             max_sigma = util::prepareDiccFileForRP(
                    _parser_conf.inputFile() + ".dicc",
                    _parser_conf.inputFile()+"["+std::to_string(_iter)+"].dicc",
                    sizeof(uint32_t),
                    _parser->results._max_alph_val
                    );
#ifdef MEASURE_TIME
                auto end = std::chrono::high_resolution_clock::now();
                auto elapse = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                CLogger::GetLogger()->model[_parser_conf.inputFile() + ":posprocess-dicc:time"] = elapse;
            }
#endif


            maxSigmaIt.emplace_back(_parser->results._max_alph_val,max_sigma);

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
        std::pair<uint32_t,uint32_t> postprocess() {
            // create a file for the final rules...
            std::fstream R(_filename + ".R", std::ios::out|std::ios::binary);

            // write the initial sigma value
            uint32_t initial_sigma = maxSigmaIt.front().first;
            R.write((const char *)&initial_sigma,sizeof(uint32_t));

            // keep the rules ids.
            std::map<uint32_t,uint32_t> rule_map;
            // last id of the rules assigned
            uint32_t offset_rules = initial_sigma + 1;
            uint32_t total_rules = 0;


            for (uint32_t i = 0; i < repairFilesR.size() - 1 ; ++i) {
#ifdef DEBUG_PRINT
                std::cout<<"Posprocessing files R and C of dicc:"<<i+1<<std::endl;
#endif
                //open files C and R
                std::fstream fileR(repairFilesR[i], std::ios::in|std::ios::binary);
                std::fstream fileC(repairFilesC[i], std::ios::in|std::ios::binary);
                //process file R and return the number of rules
                uint32_t n_rules_level = postprocessDiccRFile(fileR,R,offset_rules,maxSigmaIt[i].first,rule_map);
                //process file C (Create rules for each phrase in the level)
                postprocessDiccCFile(fileC,R,n_rules_level,offset_rules,maxSigmaIt[i].first,maxSigmaIt[i].second,rule_map);
                /* Update the offset of the rules adding the number of rules created in this iteration**/
                offset_rules += n_rules_level;
                total_rules += n_rules_level;

            }


            /**
            *  Process last parser file stored in repairFilesR.last
            * */

            std::fstream C(_filename + ".C", std::ios::out|std::ios::binary);
            std::pair<uint32_t,uint32_t> p = posprocessingFinalParseFile(R,C,offset_rules,initial_sigma,rule_map);


#ifdef MEASURE_TIME
            CLogger::GetLogger()->model["grammar:size"] = 2*(total_rules+p.first)+p.second;
            CLogger::GetLogger()->model["grammar:rules"] += total_rules+p.first;
#endif

            return std::make_pair(total_rules+p.first,p.second);
        }


        protected:


        /**
         * Read the rules and update the ids using the current offset
         * and the map
         * return the number of rules
        */
        uint32_t postprocessDiccRFile(
                std::fstream& file,
                std::fstream& R,
                const uint32_t& offset_rules,
                const uint32_t &min_sigma,
                std::map<uint32_t,uint32_t>& rule_map
                ){

            uint32_t maxsigma = 0;
            file.read((char*)&maxsigma,sizeof(int));

            uint32_t X,Y,c = 0;
            while(!file.eof() && file.read((char*)&X,sizeof(uint32_t)) && file.read((char*)&Y,sizeof(uint32_t)))
            {
                // If X is less than min_sigma then is an element of the initial alph
//                if(X > min_sigma){
                    /* If X is an element of the alp we change it by the corresponding non-terminal**/
                    if(X < maxsigma)
                        X = rule_map.empty()?X:rule_map[X];
                    else
                        X = (X - maxsigma) + offset_rules;
//                }
                //Same for Y
//                if(Y > min_sigma){
                    /* If X is an element of the alp we change it by the corresponding non-terminal**/
                    if(Y < maxsigma)
                        Y = rule_map.empty()?Y:rule_map[Y];
                    else
                        Y = (Y - maxsigma) + offset_rules;
//                }

                /*Write in the file*/
                R.write((char*)&X,sizeof(uint32_t));
                R.write((char*)&Y,sizeof(uint32_t));


#ifdef DEBUG_PRINT

                if(_parser_conf.bytesToRead() == 1){

                    char p1 = X;
                    char p2 = Y;
                    std::cout<<c+offset_rules+1<<"-<";
                    if(X <= 67)
                        std::cout<<p1;
                    else std::cout<<X;
                    std::cout<<",";
                    if(Y <= 67)
                        std::cout<<p2;
                    else std::cout<<Y;
                    std::cout<<">"<<std::endl;
                } else{

                    std::cout<<c+offset_rules+1<<"-<"<<X<<","<<Y<<">"<<std::endl;
                }

#endif

                c++;
            }
            return c;
        }


        /**
         * Read the C file and build a rule for each phrase
         * Each phrase is delimited in the sequence by an element between min_sigma and max_sigma ......$.....$......$......$
         * Update de map_rules for the next level
         * the rules created will be the alphabeth of the next level
         * */
        void postprocessDiccCFile(
                std::fstream& file,
                std::fstream& R,
                uint32_t& n_rules_level,
                const uint32_t& offset_rules,
                const uint32_t &min_sigma,
                const uint32_t &max_sigma,
                std::map<uint32_t,uint32_t>& rule_map
                ){
                    /* store the current rule*/
                    std::vector<uint32_t> X;
                    /* read the current rule*/
                    uint32_t rule = 0;
                    uint32_t level_rule_id = 0;
                    std::map<uint32_t,uint32_t> rule_map_level;

#ifdef DEBUG_PRINT

            std::cout<<"Parser sequence:"<<std::endl;
#endif
                    while(!file.eof() && file.read((char*)&rule,sizeof(uint32_t)))
                    {
                        // rule is an alph or a repair rule (NOT an artificial symbol!!!)
                        if(rule <= min_sigma || rule > max_sigma) {
                            /*if it is a valid simbol add it to the current rule*/
                            if(rule <= min_sigma) // if it is a alph symbol mapping
                            {
                                uint32_t xx =  rule_map.empty()?rule:rule_map[rule];
                                X.push_back(xx);

#ifdef DEBUG_PRINT


                                if(xx < 67)
                                    std::cout<<char(xx)<<" ";
                                else
                                    std::cout<<xx<<" ";

#endif

                            }
                            else // if it is a new rule compute its real id
                            {
                                uint32_t xx =  rule - max_sigma - 1 + offset_rules;
                                X.push_back(xx);

#ifdef DEBUG_PRINT
                                std::cout<<xx<<" ";
#endif
                            }
                        }else{


#ifdef DEBUG_PRINT

                            std::cout<<std::endl;
#endif
                            //create a binary rules for each phrase
                            if(X.size() > 1)
                            {
                                uint32_t last = X.size()-1, i = 0;
                                while(i < X.size()-1)
                                {
                                    if(i == last){
                                        X.push_back(X[last]);
                                        last = X.size()-1;
                                        ++i;
                                    }
                                    else{

                                        X.push_back( n_rules_level + offset_rules );

                                        if(i+1 == last)
                                            last = X.size()-1;

                                        ++n_rules_level;

                                        R.write((char*)&X[i],4);
                                        R.write((char*)&X[i+1],4);

#ifdef DEBUG_PRINT
                                        std::cout<<n_rules_level - 1 + offset_rules<<"-<"<<X[i]<<","<<X[i+1]<<">"<<std::endl;
#endif
                                        i+=2;


                                    }



                                }
                                X.clear();
                                rule_map_level[++level_rule_id] = n_rules_level - 1 + offset_rules;

                            }else{
                                rule_map_level[++level_rule_id] = X.front();
                                X.clear();
                            }
                        }
                    }
#ifdef DEBUG_PRINT

                    std::cout<<std::endl;
#endif
                    rule_map.clear();
                    rule_map = rule_map_level;
            }



        std::pair<uint32_t,uint32_t> posprocessingFinalParseFile(
                std::fstream& R,
                std::fstream& C,
                uint32_t& offset_rules,
                const uint32_t& initial_sigma,
                std::map<uint32_t,uint32_t>& rule_map
                ){


            /*Updating rules*/
            std::fstream r_file(repairFilesR.back(),std::ios::in);
            uint32_t  maxsigma = 0, n_rules = 0;

#ifdef DEBUG_PRINT

            std::cout<<"FINAL SEQ RULES"<<std::endl;
#endif
            {
                r_file.read((char*)&maxsigma,4);
                /**reading rules*/
                uint32_t X,Y;
                while(!r_file.eof() && r_file.read((char*)&X,sizeof(uint32_t)) && r_file.read((char*)&Y,sizeof(uint32_t )))
                {
                    /* If X is an element of the alp we change it by the corresponding non-terminal**/
                    if(X < maxsigma)X = rule_map[X];
                        /* If X is a rule we add it the corresponding offset**/
                    else X = X - maxsigma + offset_rules;
                    /*Same for Y**/
                    if(Y < maxsigma)Y = rule_map[Y];
                    else Y = Y - maxsigma + offset_rules;
                    /*Write in the file*/
                    R.write((char*)&X,sizeof(uint32_t));
                    R.write((char*)&Y,sizeof(uint32_t));
                    /*Increase the number of rules*/

#ifdef DEBUG_PRINT
                    char p1 = X;
                    char p2 = Y;
                    std::cout<<offset_rules+n_rules+1<<"-<";
                    if(X <= initial_sigma)
                        std::cout<<p1;
                    else std::cout<<X;
                    std::cout<<",";
                    if(Y <= initial_sigma)
                        std::cout<<p2;
                    else std::cout<<Y;
                    std::cout<<">"<<std::endl;

#endif
                    ++n_rules;
                }
            }

            uint32_t len = 0;
            {
                /*Updating first rule*/
                std::fstream c_file(repairFilesC.back(),std::ios::in);
                uint32_t X = 0;

#ifdef DEBUG_PRINT

                std::cout<<"FINAL SEQ C"<<std::endl;
#endif
                while(!c_file.eof() && c_file.read((char*)&X,sizeof(uint32_t)))
                {
                    /* If X is an element of the alp we change it by the corresponding non-terminal**/
                    if(X < maxsigma) X = rule_map[X];
                    /* If X is a rule we add it the corresponding offset**/
                    else X = X - maxsigma + offset_rules;
                    C.write((char*)&X,sizeof(uint32_t));

#ifdef DEBUG_PRINT

                    if(X < initial_sigma) std::cout<<char(X)<<" ";
                    else std::cout<<X<<" ";
#endif
                    len++;
                }


#ifdef DEBUG_PRINT

                std::cout<<std::endl;
#endif
            }

            return std::make_pair(n_rules,len);

        }

    };











    template <>
    class RePairRecursiveConfig <
            uint32_t,
            DummyRepair,
            RandomPermutationParser
    >
    {

    public:
        RePairRecursiveConfig(){
            _iter = 0;
        };


        RePairRecursiveConfig(const std::string &file, const DummyRepair& c, const uint32_t& max_iter,const uint32_t& th_initial_seq){

            _filename = file;
            _iter = 0;
            _compressor = c;  //dummy compressor
            _max_iter = max_iter; // max number of iterations
            _th_initial_seq = th_initial_seq; // threshold for sequence len
            _size_seq = std::numeric_limits<uint32_t>::max();

        };
        RePairRecursiveConfig(const RePairRecursiveConfig <
                uint32_t,
                DummyRepair,
                RandomPermutationParser
        >& RPC){

            /**
             * Parameters
             * */

            _filename = RPC._filename;
            _iter = RPC._iter;
            _max_iter = RPC._max_iter;
            _th_initial_seq = RPC._th_initial_seq;
            _compressor = RPC._compressor;
            _size_seq = RPC._size_seq;

        }
        RePairRecursiveConfig& operator=(const RePairRecursiveConfig <
                uint32_t,
                DummyRepair,
                RandomPermutationParser
        >& RPC){

            _filename = RPC._filename;
            _iter = RPC._iter;
            _max_iter = RPC._max_iter;
            _th_initial_seq = RPC._th_initial_seq;
            _compressor = RPC._compressor;
            _size_seq = RPC._size_seq;
            return *this;
        }
        virtual ~RePairRecursiveConfig() {
            cleanTmpFiles();
        }


    protected:
        /**
         * Parameters
         * */
        std::string _filename;
        uint32_t  _iter;
        uint32_t  _max_iter;
        uint32_t  _th_initial_seq;

        /**
         * internal data
         * */
        uint32_t  _size_seq;
        std::vector<std::pair<uint32_t,uint32_t>> maxSigmaIt; // store the sigma alph (max alp, max artificial add) used in each it
        std::vector<std::string> diccFiles;
        std::vector<std::string> parseFiles;
        std::vector<std::string> repairFilesR;
        std::vector<std::string> repairFilesC;

        DummyRepair _compressor;
        RandomPermutationParser _parser;

        void cleanTmpFiles(){

            for(std::string s:diccFiles){
                std::string rm_cmd_old_dicc = "rm "+ s;
                if(system(rm_cmd_old_dicc.c_str()) < 0  ){
                    std::cout<<"[ERROR] Can't remove tmp file: "+ s;
                }
            }
            for(std::string s:parseFiles){
                std::string rm_cmd_old_dicc = "rm "+ s;
                if(system(rm_cmd_old_dicc.c_str()) < 0  ){
                    std::cout<<"[ERROR] Can't remove tmp file: "+ s;
                }
            }
            for(std::string s:repairFilesR){
                std::string rm_cmd_old_dicc = "rm "+ s;
                if(system(rm_cmd_old_dicc.c_str()) < 0  ){
                    std::cout<<"[ERROR] Can't remove tmp file: "+ s;
                }
            }
            for(std::string s:repairFilesC){
                std::string rm_cmd_old_dicc = "rm "+ s;
                if(system(rm_cmd_old_dicc.c_str()) < 0  ){
                    std::cout<<"[ERROR] Can't remove tmp file: "+ s;
                }
            }
        }

        std::string file() const { return _filename;}

        uint32_t setIter(const uint32_t& i){ _iter = i; return _iter;}
        uint32_t iter() const { return _iter;}
        uint32_t increaseIter(){ ++_iter; return _iter;}
        uint32_t thInitialSeq() const { return _th_initial_seq;}
        uint32_t sizeSeq() const { return _size_seq;}
        uint32_t setSizeSeq( const uint32_t& s) { _size_seq = s; return _size_seq;}


    public:

        bool stopCondition(){
            return ( _iter > _max_iter ) || ( _size_seq < _th_initial_seq );
        }
        bool firstIteration(){ return !_iter; }

        /**
       * Apply repair to all files
       * */
        void compressor() {
            //apply repair and store repair files

            for(std::string s:diccFiles){

#ifdef MEASURE_TIME
                {
                    auto start = std::chrono::high_resolution_clock::now();
#endif

                if(_compressor.apply(s) > 0){
                    repairFilesR.push_back(s + ".R");
                    repairFilesC.push_back(s + ".C");
                }
                else
                    throw "[ERROR] RePair compression fail dicc file " + s;
#ifdef MEASURE_TIME
                    auto end = std::chrono::high_resolution_clock::now();
                    auto elapse = std::chrono::duration_cast<std::chrono::microseconds>( end - start).count();
                    CLogger::GetLogger()->model[s + ":repair:time"] = elapse;

                }
#endif
            }

#ifdef MEASURE_TIME
            {
                auto start = std::chrono::high_resolution_clock::now();
#endif
            std::string last_parse_file = parseFiles.back();
            if(_compressor.apply(last_parse_file) > 0){
                repairFilesR.push_back(last_parse_file + ".R");
                repairFilesC.push_back(last_parse_file + ".C");
            } else throw "[ERROR] RePair compression fail last parser file";
#ifdef MEASURE_TIME
                auto end = std::chrono::high_resolution_clock::now();
                auto elapse = std::chrono::duration_cast<std::chrono::microseconds>( end - start).count();
                CLogger::GetLogger()->model[last_parse_file + ":repair:time"] = elapse;

            }
#endif
        };
        /**
        * partitioner method must create two files file_dicc and file_parse as integers
        * and Preprocess  dicc file in order to be able apply repair
        * */

        virtual void parseIt()
        {
            uint32_t byte = 1;
            uint32_t sigma = 256;
            if(firstIteration()){

                _parser.params.filename = _filename;

            }else{

                byte = 4;
                sigma = _parser.params.dicc_len;
                _parser.params.reset();
                _parser.params.filename = parseFiles[_iter-1];

            }
            //parse current file

#ifdef MEASURE_TIME
            {
                auto start = std::chrono::high_resolution_clock::now();
#endif
            _parser.parser(_parser.params.filename,sigma,byte);

#ifdef MEASURE_TIME
                auto end = std::chrono::high_resolution_clock::now();
                auto elapse = std::chrono::duration_cast<std::chrono::microseconds>( end - start).count();
                CLogger::GetLogger()->model[_parser.params.filename + ":parse:time"] = elapse;

            }
#endif
            //std::cout<<"hash_table_len:"<<_parser.hash_table.size()<<std::endl;
            //std::cout<<"dicc_len      :"<<_parser.params.dicc_len  <<std::endl;
            _parser.hash_table.clear();
            //posprocess dictionary
            _size_seq = _parser.params.seq_len;
            uint32_t max_sigma = 0;
#ifdef MEASURE_TIME
            {
                auto start = std::chrono::high_resolution_clock::now();
#endif
            max_sigma = util::prepareDiccFileForRP(
                    _parser.params.filename + ".dicc",
                    _parser.params.filename+"["+std::to_string(_iter)+"].dicc",
                    sizeof(uint32_t),
                    _parser.params.max_sigma
            );
#ifdef MEASURE_TIME
            auto end = std::chrono::high_resolution_clock::now();
            auto elapse = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            CLogger::GetLogger()->model[_parser.params.filename + ":posprocess-dicc:time"] = elapse;
        }
#endif


            maxSigmaIt.emplace_back(_parser.params.max_sigma,max_sigma);

            //store the dicc file for later compression
            diccFiles.push_back(_parser.params.filename+"["+std::to_string(_iter)+"].dicc");
            //store the parse file, this file will be the next to parse
            //this files will be removed in the destructor
            parseFiles.push_back(_parser.params.filename + ".parse");

            increaseIter();
        }
        /**
         *  Postprocess the files in case that it will be necesary
         * */
        std::pair<uint32_t,uint32_t> postprocess() {
            // create a file for the final rules...
            std::fstream R(_filename + ".rR", std::ios::out|std::ios::binary);

            // write the initial sigma value
            uint32_t initial_sigma = maxSigmaIt.front().first;
            R.write((const char *)&initial_sigma,sizeof(uint32_t));

            // keep the rules ids.
            std::map<uint32_t,uint32_t> rule_map;
            // last id of the rules assigned
            uint32_t offset_rules = initial_sigma + 1;
            uint32_t total_rules = 0;


            for (uint32_t i = 0; i < repairFilesR.size() - 1 ; ++i) {
#ifdef DEBUG_PRINT
                std::cout<<"Posprocessing files R and C of dicc:"<<i+1<<std::endl;
#endif
                //open files C and R
                std::fstream fileR(repairFilesR[i], std::ios::in|std::ios::binary);
                std::fstream fileC(repairFilesC[i], std::ios::in|std::ios::binary);
                //process file R and return the number of rules
                uint32_t n_rules_level = postprocessDiccRFile(fileR,R,offset_rules,maxSigmaIt[i].first,rule_map);
                //process file C (Create rules for each phrase in the level)
                postprocessDiccCFile(fileC,R,n_rules_level,offset_rules,maxSigmaIt[i].first,maxSigmaIt[i].second,rule_map);
                /* Update the offset of the rules adding the number of rules created in this iteration**/
                offset_rules += n_rules_level;
                total_rules += n_rules_level;
            }

            /**
            *  Process last parser file stored in repairFilesR.last
            * */

            std::fstream C(_filename + ".rC", std::ios::out|std::ios::binary);
            std::pair<uint32_t,uint32_t> p = posprocessingFinalParseFile(R,C,offset_rules,initial_sigma,rule_map);

#ifdef MEASURE_TIME
            CLogger::GetLogger()->model["grammar:size"] = 2*(total_rules+p.first)+p.second;
            CLogger::GetLogger()->model["grammar:rules"] += total_rules + 1;
#endif

            return std::make_pair(total_rules+p.first,p.second);
        }


    protected:


        /**
         * Read the rules and update the ids using the current offset
         * and the map
         * return the number of rules
        */
        uint32_t postprocessDiccRFile(
                std::fstream& file,
                std::fstream& R,
                const uint32_t& offset_rules,
                const uint32_t &min_sigma,
                std::map<uint32_t,uint32_t>& rule_map
        ){

            uint32_t maxsigma = 0;
            file.read((char*)&maxsigma,sizeof(int));

            uint32_t X,Y,c = 0;
            while(!file.eof() && file.read((char*)&X,sizeof(uint32_t)) && file.read((char*)&Y,sizeof(uint32_t)))
            {
                // If X is less than min_sigma then is an element of the initial alph
//                if(X > min_sigma){
                /* If X is an element of the alp we change it by the corresponding non-terminal**/
                if(X < maxsigma)
                    X = rule_map.empty()?X:rule_map[X];
                else
                    X = (X - maxsigma) + offset_rules;
//                }
                //Same for Y
//                if(Y > min_sigma){
                /* If X is an element of the alp we change it by the corresponding non-terminal**/
                if(Y < maxsigma)
                    Y = rule_map.empty()?Y:rule_map[Y];
                else
                    Y = (Y - maxsigma) + offset_rules;
//                }

                /*Write in the file*/
                R.write((char*)&X,sizeof(uint32_t));
                R.write((char*)&Y,sizeof(uint32_t));


#ifdef DEBUG_PRINT

                if(_parser_conf.bytesToRead() == 1){

                    char p1 = X;
                    char p2 = Y;
                    std::cout<<c+offset_rules+1<<"-<";
                    if(X <= 67)
                        std::cout<<p1;
                    else std::cout<<X;
                    std::cout<<",";
                    if(Y <= 67)
                        std::cout<<p2;
                    else std::cout<<Y;
                    std::cout<<">"<<std::endl;
                } else{

                    std::cout<<c+offset_rules+1<<"-<"<<X<<","<<Y<<">"<<std::endl;
                }

#endif

                c++;
            }
            return c;
        }


        /**
         * Read the C file and build a rule for each phrase
         * Each phrase is delimited in the sequence by an element between min_sigma and max_sigma ......$.....$......$......$
         * Update de map_rules for the next level
         * the rules created will be the alphabeth of the next level
         * */
        void postprocessDiccCFile(
                std::fstream& file,
                std::fstream& R,
                uint32_t& n_rules_level,
                const uint32_t& offset_rules,
                const uint32_t &min_sigma,
                const uint32_t &max_sigma,
                std::map<uint32_t,uint32_t>& rule_map
        ){
            /* store the current rule*/
            std::vector<uint32_t> X;
            /* read the current rule*/
            uint32_t rule = 0;
            uint32_t level_rule_id = 0;
            std::map<uint32_t,uint32_t> rule_map_level;

#ifdef DEBUG_PRINT

            std::cout<<"Parser sequence:"<<std::endl;
#endif
            while(!file.eof() && file.read((char*)&rule,sizeof(uint32_t)))
            {
                // rule is an alph or a repair rule (NOT an artificial symbol!!!)
                if(rule <= min_sigma || rule > max_sigma) {
                    /*if it is a valid simbol add it to the current rule*/
                    if(rule <= min_sigma) // if it is a alph symbol mapping
                    {
                        uint32_t xx =  rule_map.empty()?rule:rule_map[rule];
                        X.push_back(xx);

#ifdef DEBUG_PRINT


                        if(xx < 67)
                                    std::cout<<char(xx)<<" ";
                                else
                                    std::cout<<xx<<" ";

#endif

                    }
                    else // if it is a new rule compute its real id
                    {
                        uint32_t xx =  rule - max_sigma - 1 + offset_rules;
                        X.push_back(xx);

#ifdef DEBUG_PRINT
                        std::cout<<xx<<" ";
#endif
                    }
                }else{


#ifdef DEBUG_PRINT

                    std::cout<<std::endl;
#endif
                    //create a binary rules for each phrase
                    if(X.size() > 1)
                    {
                        uint32_t last = X.size()-1, i = 0;
                        while(i < X.size()-1)
                        {
                            if(i == last){
                                X.push_back(X[last]);
                                last = X.size()-1;
                                ++i;
                            }
                            else{

                                X.push_back( n_rules_level + offset_rules );

                                if(i+1 == last)
                                    last = X.size()-1;

                                ++n_rules_level;

                                R.write((char*)&X[i],4);
                                R.write((char*)&X[i+1],4);


#ifdef DEBUG_PRINT
                                std::cout<<n_rules_level - 1 + offset_rules<<"-<"<<X[i]<<","<<X[i+1]<<">"<<std::endl;
#endif
                                i+=2;


                            }



                        }
                        X.clear();
                        rule_map_level[++level_rule_id] = n_rules_level - 1 + offset_rules;

                    }else{
                        rule_map_level[++level_rule_id] = X.front();
                        X.clear();
                    }
                }
            }
#ifdef DEBUG_PRINT

            std::cout<<std::endl;
#endif
            rule_map.clear();
            rule_map = rule_map_level;
        }



        std::pair<uint32_t,uint32_t> posprocessingFinalParseFile(
                std::fstream& R,
                std::fstream& C,
                uint32_t& offset_rules,
                const uint32_t& initial_sigma,
                std::map<uint32_t,uint32_t>& rule_map
        ){


            /*Updating rules*/
            std::fstream r_file(repairFilesR.back(),std::ios::in);
            uint32_t  maxsigma = 0, n_rules = 0;

#ifdef DEBUG_PRINT

            std::cout<<"FINAL SEQ RULES"<<std::endl;
#endif
            {
                r_file.read((char*)&maxsigma,4);
                /**reading rules*/
                uint32_t X,Y;
                while(!r_file.eof() && r_file.read((char*)&X,sizeof(uint32_t)) && r_file.read((char*)&Y,sizeof(uint32_t )))
                {
                    /* If X is an element of the alp we change it by the corresponding non-terminal**/
                    if(X < maxsigma)X = rule_map[X];
                        /* If X is a rule we add it the corresponding offset**/
                    else X = X - maxsigma + offset_rules;
                    /*Same for Y**/
                    if(Y < maxsigma)Y = rule_map[Y];
                    else Y = Y - maxsigma + offset_rules;
                    /*Write in the file*/
                    R.write((char*)&X,sizeof(uint32_t));
                    R.write((char*)&Y,sizeof(uint32_t));
                    /*Increase the number of rules*/

#ifdef DEBUG_PRINT
                    char p1 = X;
                    char p2 = Y;
                    std::cout<<offset_rules+n_rules+1<<"-<";
                    if(X <= initial_sigma)
                        std::cout<<p1;
                    else std::cout<<X;
                    std::cout<<",";
                    if(Y <= initial_sigma)
                        std::cout<<p2;
                    else std::cout<<Y;
                    std::cout<<">"<<std::endl;

#endif
                    ++n_rules;
                }
            }

            uint32_t len = 0;
            {
                /*Updating first rule*/
                std::fstream c_file(repairFilesC.back(),std::ios::in);
                uint32_t X = 0;

#ifdef DEBUG_PRINT

                std::cout<<"FINAL SEQ C"<<std::endl;
#endif
                while(!c_file.eof() && c_file.read((char*)&X,sizeof(uint32_t)))
                {
                    /* If X is an element of the alp we change it by the corresponding non-terminal**/
                    if(X < maxsigma) X = rule_map[X];
                        /* If X is a rule we add it the corresponding offset**/
                    else X = X - maxsigma + offset_rules;
                    C.write((char*)&X,sizeof(uint32_t));

#ifdef DEBUG_PRINT

                    if(X < initial_sigma) std::cout<<char(X)<<" ";
                    else std::cout<<X<<" ";
#endif
                    len++;
                }

#ifdef DEBUG_PRINT

                std::cout<<std::endl;
#endif
            }

            return std::make_pair(n_rules,len);

        }

    };

}


#endif //RRPAIR_REPAIRRECURSIVECONFIG_H
