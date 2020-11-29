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

#define DEBUG_PRINT 1

namespace big_repair{

//    typedef uint32_t uint_t_32;
    typedef uint64_t uint_t_64;


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
        virtual bool stopCondition() = 0;
    };



    template <>
    class RePairRecursiveConfig <
             uint_t_64,
            DummyRepair,
            HashParser< HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint_t_64 ,std::string > > >
            >
    {

        typedef HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint_t_64 ,std::string > > ParseConf;
        typedef HashParser<HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint_t_64 ,std::string > > > Parse;
//        const uint_t& ws,const uint_t& b,const uint_t& m,const std::string& input, const std::string& tempDir

        public:
            RePairRecursiveConfig(){
                _iter = 0;
                _parser = nullptr;
            };


            RePairRecursiveConfig(const ParseConf& pc,const DummyRepair& c, const uint_t& max_iter,const uint_t& th_initial_seq){

                _iter = 0;
                _compressor = c;  //dummy compressor
                _filename = pc.inputFile();
                _parser_conf = pc; // paser conf
                _parser = nullptr; // pointer to the current parser
                _max_iter = max_iter; // max number of iterations
                _th_initial_seq = th_initial_seq; // threshold for sequence len
                _size_seq = std::numeric_limits<uint_t>::max();

            };
            RePairRecursiveConfig(const RePairRecursiveConfig <
                     uint_t_64,
                    DummyRepair,
                    HashParser< HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint_t_64 ,std::string > > >
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
                     uint_t_64,
                    DummyRepair,
                    HashParser< HashParserConfig< KRPSlindingWindow<>, KRPHashFunction< uint_t_64 ,std::string > > >
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
            uint_t_64  _iter;
            uint_t_64  _max_iter;
            uint_t_64  _th_initial_seq;

            /**
             * internal data
             * */
            uint_t_64  _size_seq;
            std::vector<std::pair<uint_t_64,uint_t_64>> maxSigmaIt; // store the sigma alph (max alp, max artificial add) used in each it
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

            uint_t_64 setIter(const uint_t_64 & i){ _iter = i; return _iter;}
            uint_t_64 iter() const { return _iter;}
            uint_t_64 increaseIter(){ ++_iter; return _iter;}
            uint_t_64 thInitialSeq() const { return _th_initial_seq;}
            uint_t_64 sizeSeq() const { return _size_seq;}
            uint_t_64 setSizeSeq( const uint_t_64& s) { _size_seq = s; return _size_seq;}

            void updateParserConf(){
                _parser_conf.setInputFile(parseFiles[_iter-1]);
                _parser_conf.setBytesToRead(sizeof(uint_t_64));
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
                if(_compressor.apply(s) > 0){
                    repairFilesR.push_back(s + ".R");
                    repairFilesC.push_back(s + ".C");
                }
                else
                    throw "[ERROR] RePair compression fail dicc file " + s;

            }
            std::string last_parse_file = parseFiles.back();
            if(_compressor.apply(last_parse_file) > 0){
                repairFilesR.push_back(last_parse_file + ".R");
                repairFilesC.push_back(last_parse_file + ".C");
            } else throw "[ERROR] RePair compression fail last parser file";

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
            _parser->parseFileSM();
            //posprocess dictionary
            _size_seq = _parser->results._seq_len;
            uint_t_64 max_sigma = util::prepareDiccFileForRP(
                    _parser_conf.inputFile() + ".dicc",
                    _parser_conf.inputFile()+"["+std::to_string(_iter)+"].dicc",
                    sizeof(uint_t_64),
                    _parser->results._max_alph_val
                    );


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
        std::pair<uint_t,uint_t> postprocess() {
            // create a file for the final rules...
            std::fstream R(_filename + ".R", std::ios::out|std::ios::binary);

            // write the initial sigma value
            uint_t_64 initial_sigma = maxSigmaIt.front().first;
            R.write((const char *)&initial_sigma,sizeof(uint_t_64));


#ifdef DEBUG_PRINT
std::cout<<"initial sigma value:"<<initial_sigma<<std::endl;
#endif
            // keep the rules ids.
            std::map<uint_t_64,uint_t_64> rule_map;
            // last id of the rules assigned
            uint_t_64 offset_rules = initial_sigma + 1;
            uint_t_64 total_rules = 0;


            for (uint_t_64 i = 0; i < repairFilesR.size() - 1 ; ++i) {
#ifdef DEBUG_PRINT
std::cout<<"Posprocessing files R and C of dicc:"<<i+1<<std::endl;
std::cout<<"Posprocessing R :"<<repairFilesR[i]<<std::endl;
std::cout<<"Posprocessing C :"<<repairFilesC[i]<<std::endl;
#endif
                //open files C and R
                std::fstream fileR(repairFilesR[i], std::ios::in|std::ios::binary);
                std::fstream fileC(repairFilesC[i], std::ios::in|std::ios::binary);
                //process file R and return the number of rules
                uint_t_64 n_rules_level = postprocessDiccRFile(fileR,R,offset_rules,maxSigmaIt[i].first,rule_map);
                //process file C (Create rules for each phrase in the level)
                postprocessDiccCFile(fileC,R,n_rules_level,offset_rules,maxSigmaIt[i].first,maxSigmaIt[i].second,rule_map);
                /* Update the offset of the rules adding the number of rules created in this iteration**/
                offset_rules += n_rules_level;
                total_rules += n_rules_level;
#ifdef DEBUG_PRINT
std::cout<<"*********************************************************************"<<std::endl;
#endif
            }

            /**
            *  Process last parser file stored in repairFilesR.last
            * */



            std::fstream C(_filename + ".C", std::ios::out|std::ios::binary);
            std::pair<uint_t_64,uint_t_64> p = posprocessingFinalParseFile(R,C,offset_rules,initial_sigma,rule_map);
            return std::make_pair(total_rules+p.first,p.second);
        }


        protected:


        /**
         * Read the rules and update the ids using the current offset
         * and the map
         * return the number of rules
        */
        uint_t_64 postprocessDiccRFile(
                std::fstream& file,
                std::fstream& R,
                const uint_t_64& offset_rules,
                const uint_t_64 &min_sigma,
                std::map<uint_t_64,uint_t_64>& rule_map
                ){

            uint_t_64 maxsigma = 0;
            file.read((char*)&maxsigma,sizeof(int));

#ifdef DEBUG_PRINT
std::cout<<"postprocessDiccRFile"<<std::endl;
std::cout<<"postprocessDiccRFile offset_rules: "<<offset_rules<<std::endl;
std::cout<<"postprocessDiccRFile min_sigma: "<<min_sigma<<std::endl;
std::cout<<"postprocessDiccRFile maxsigma: "<<maxsigma<<std::endl;
std::cout<<"postprocessDiccRFile ruleMapLen: "<<rule_map.size()<<std::endl;
#endif
            int XX ,YY ;
            uint_t_64 c = 0;
            // reading repair pairs size(int)
            while(!file.eof() && file.read((char*)&XX,sizeof(int)) && file.read((char*)&YY,sizeof(int)))
            {
                uint_t_64 X = XX, Y = YY;
                std::cout<<XX<<"-"<<YY<<std::endl;
                /**
                 * If X is less than min_sigma then is an element of the initial alph
                 * */
//                if(X > min_sigma){
                    /* If X is an element of the alp we change it by the corresponding non-terminal**/
                    if(X < maxsigma)
                          X = rule_map.empty()?X:rule_map[X];
                    else{
                        X = (X - maxsigma) + offset_rules;
                    }
//                }
                /**
                 * Same for Y
                 * */
//                if(Y > min_sigma){
                /* If X is an element of the alp we change it by the corresponding non-terminal**/
                    if(Y < maxsigma)
                          Y = rule_map.empty()?Y:rule_map[Y];
                    else
                        Y = (Y - maxsigma) + offset_rules;
//                }

                /*Write in the file*/
                R.write((char*)&X,sizeof(uint_t_64));
                R.write((char*)&Y,sizeof(uint_t_64));



#ifdef DEBUG_PRINT

#endif
                c++;
            }
#ifdef DEBUG_PRINT
std::cout<<"postprocessDiccRFile number of rules:"<<c<<std::endl;
#endif
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
                uint_t_64 & n_rules_level,
                const uint_t_64 & offset_rules,
                const uint_t_64  &min_sigma,
                const uint_t_64  &max_sigma,
                std::map<uint_t_64 ,uint_t_64 >& rule_map
                ){
                    /* store the current rule*/
                    std::vector<uint_t_64> X;
                    /* read the current rule*/
                    int  _rule = 0;
                    uint_t_64  level_rule_id = 0;
                    std::map<uint_t_64 ,uint_t_64 > rule_map_level;

#ifdef DEBUG_PRINT
std::cout<<"postprocessDiccCFile "<<std::endl;
std::cout<<"postprocessDiccCFile n_rules_level "<<n_rules_level<<std::endl;
std::cout<<"postprocessDiccCFile offset_rules "<<offset_rules<<std::endl;
std::cout<<"postprocessDiccCFile min_sigma "<<min_sigma<<std::endl;
std::cout<<"postprocessDiccCFile max_sigma "<<max_sigma<<std::endl;
std::cout<<"postprocessDiccCFile ruleMapLen: "<<rule_map.size()<<std::endl;
//            std::cout<<"Parser sequence:"<<std::endl;
#endif

                    while(!file.eof() && file.read((char*)&_rule,sizeof(int)))
                    {
                        uint_t_64 rule = _rule;
                        // rule is an alph or a repair rule (NOT an artificial symbol!!!)
                        if(rule <= min_sigma || rule > max_sigma) {
                            /*if it is a valid simbol add it to the current rule*/
                            if(rule <= min_sigma) // if it is a alph symbol mapping
                            {
                                uint_t_64 xx =  rule_map.empty()?rule:rule_map[rule];
                                X.push_back(xx);

#ifdef DEBUG_PRINT
if(xx < 71)
    std::cout<<char(xx)<<" ";
else
    std::cout<<xx<<" ";
#endif

                            }
                            else // if it is a new rule compute its real id
                            {
                                uint_t_64 xx =  rule - max_sigma - 1 + offset_rules;
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
                                uint_t_64 last = X.size()-1, i = 0;
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

                                        R.write((char*)&X[i],sizeof(uint_t_64));
                                        R.write((char*)&X[i+1],sizeof(uint_t_64));

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
std::cout<<"postprocessDiccCFile numero de reglas:"<<n_rules_level<<std::endl;
#endif
                    rule_map.clear();
                    rule_map = rule_map_level;
            }



        std::pair<uint_t_64 ,uint_t_64 > posprocessingFinalParseFile(
                std::fstream& R,
                std::fstream& C,
                uint_t_64 & offset_rules,
                const uint_t_64 & initial_sigma,
                std::map<uint_t_64 ,uint_t_64 >& rule_map
                ){


            /*Updating rules*/
            std::fstream r_file(repairFilesR.back(),std::ios::in);
            uint_t_64   maxsigma = 0, n_rules = 0;

            {
                r_file.read((char*)&maxsigma,sizeof(int));

#ifdef DEBUG_PRINT
std::cout<<"posprocessingFinalParseFile"<<std::endl;
std::cout<<"posprocessingFinalParseFile file: "<<repairFilesR.back()<<std::endl;
std::cout<<"posprocessingFinalParseFile offset_rules "<<offset_rules<<std::endl;
std::cout<<"posprocessingFinalParseFile initial_sigma "<<initial_sigma<<std::endl;
std::cout<<"posprocessingFinalParseFile max_sigma "<<maxsigma<<std::endl;
#endif
                /**reading rules*/
                int  XX = 0,YY = 0;
                while(!r_file.eof() && r_file.read((char*)&XX,sizeof(int)) && r_file.read((char*)&YY,sizeof(int )))
                {
                    auto  X = (uint_t_64)XX,Y = (uint_t_64)YY;

                    /* If X is an element of the alp we change it by the corresponding non-terminal**/
                    if(X < maxsigma)X = rule_map[X];
                        /* If X is a rule we add it the corresponding offset**/
                    else X = X - maxsigma + offset_rules;
                    /*Same for Y**/
                    if(Y < maxsigma)Y = rule_map[Y];
                    else Y = Y - maxsigma + offset_rules;
                    /*Write in the file*/
                    R.write((char*)&X,sizeof(uint_t_64));
                    R.write((char*)&Y,sizeof(uint_t_64));
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
                    XX = 0;
                    YY = 0;
                }
            }

            uint_t_64 len = 0;
            {
                /*Updating first rule*/
                std::fstream c_file(repairFilesC.back(),std::ios::in);
                int XX = 0;

#ifdef DEBUG_PRINT
std::cout<<"FINAL SEQ C"<<std::endl;
std::cout<<"posprocessingFinalParseCFile file: "<<repairFilesC.back()<<std::endl;
#endif
                while(!c_file.eof() && c_file.read((char*)&XX,sizeof(int)))
                {
                    uint_t_64 X = XX;
                    if(rule_map.find(X) == rule_map.end())
                        std::cout<<"rule doesn't found\n"<<X<<std::endl;
                    if(!X) continue;
                    /* If X is an element of the alp we change it by the corresponding non-terminal**/
                    if(X < maxsigma) X = rule_map[X];
                    /* If X is a rule we add it the corresponding offset**/
                    else X = X - maxsigma + offset_rules;
                    C.write((char*)&X,sizeof(uint_t_64));

#ifdef DEBUG_PRINT
if(X < initial_sigma) std::cout<<char(X)<<" ";
else std::cout<<X<<" ";
#endif
                    len++;
                    XX = 0;
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
