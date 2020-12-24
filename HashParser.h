//
// Created by ale on 01-07-20.
//

#ifndef RRPAIR_HASHPARSER_H
#define RRPAIR_HASHPARSER_H

#include <memory>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include "CLogger.h"

//#include <sdsl/int_vector.hpp>

//#define DEBUG_PRINT 1
//#define CHECK_COLLISION


namespace big_repair{

/*
 * This class read a file n-byte by n-bytes and create a dictionary of symbols and the sequence corresponding to the file
 */

    template <
            typename C,     //Config object
            typename hash_type = uint64_t
    >
    class HashParser {

    public:
        //todo ver como implementar stats!!
        struct stats{
            uint32_t _max_alph_val;
            uint32_t _seq_len;
            uint32_t _dicc_len;

            stats(){
                _max_alph_val = 0;
                _seq_len = 0;
                _dicc_len = 0;

            }
            void print(){


#ifdef DEBUG_PRINT
                std::cout<<"-------------------Parser Data------------------------------\n";
                std::cout<<"Max value in alph:" << _max_alph_val << std::endl;
                std::cout<<"Initial Sequence len:" << _seq_len << std::endl;
                std::cout<<"Dicctionary len:" << _dicc_len << std::endl;
#endif

            }

        };
        stats results;

    private:
        C* config;

#ifdef CHECK_COLLISION
        std::map<hash_type,std::vector<uint32_t>> coll_map;
#else
        std::set<hash_type> coll_map;
#endif
        std::unordered_map<hash_type,uint32_t> rank_hash;
        std::vector<uint32_t> compressed_seq;

#ifdef DEBUG_PRINT
    bool byte;
#endif


    public:

        HashParser() {
#ifdef CHECK_COLLISION
            std::cout<<"CHECKING COLLISIONS"<<std::endl;
#endif

            config = nullptr;

        }
        HashParser(const C& c) {
            this->config = new C(c);
        }
        HashParser(const HashParser & HP) {
            this->config = HP.config;
        }

        HashParser& operator = (const HashParser& HP){
            config = new C(HP.config);
            return *this;
        }

        ~HashParser() {
            if(config != nullptr) delete config;
        }

        // add a phrase to the dicc and to the sequence / check for collisions / write in the files
        void addWord(std::vector<uint32_t>& word, bool constrain = true){
            uint len = word.size();
            //check the word is big enough
            if (len < config->getWindow()->sizeWindow() && constrain){
                throw "windows to small";
//                return;
            }
            const char* ptr = (const char*) word.data();
            // compute the hash of the string
            hash_type hash = config->hashFunction()->apply(ptr,word.size()*sizeof(int));
            //search the has in the dicc
            auto it = coll_map.find(hash);

            if (it == coll_map.end()) {
                // new phrase
                rank_hash[hash] = coll_map.size() + 1;
                compressed_seq.push_back(coll_map.size() + 1);

                results._dicc_len++;
#ifdef CHECK_COLLISION
                // insert the hash and the word to check collisions in the map
                coll_map[hash] = std::move(word);
#else
                coll_map.insert(hash);
#endif
            }else{
                // already seen phrase
#ifdef CHECK_COLLISION
                //check collision
                if(coll_map[hash] != word){
                    word.clear();
                    throw "COLLISION FOUND";
                }
#endif
                word.clear();
                compressed_seq.push_back(rank_hash[hash]);

            }
            results._seq_len++;
        }

        void addWord(std::vector<uint32_t>& word, std::fstream& dFile, std::fstream& pFile, bool constrain = true){

            uint len = word.size();
            //check the word is big enough
            if (len < config->getWindow()->sizeWindow() && constrain){
                throw "windows to small";
//                return;
            }
            const char* ptr = (const char*) word.data();
            // compute the hash of the string
            hash_type hash = config->hashFunction()->apply(ptr,word.size()*sizeof(uint32_t));
            //search the has in the dicc
            auto it = coll_map.find(hash);

            if (it == coll_map.end()) {
                // new phrase

#ifdef DEBUG_PRINT
                std::cout<<coll_map.size() + 1<<"-";
                for(int i:word){
                    if(byte)
                        std::cout<<(char)i;
                    else
                        std::cout<<i;
                }
                std::cout<<std::endl;

                std::cout<<coll_map.size() + 1<<"- p "<<std::endl;

#endif

                results._dicc_len++;
                //write the phrase in the dictionary file
                dFile.write(ptr,word.size()*sizeof(uint32_t));
                //put 0 between phrases
                uint32_t z = 0;
                dFile.write((const char *)&z,sizeof(uint32_t));

                rank_hash[hash] = coll_map.size() + 1;
                uint32_t v = rank_hash[hash];
                // write the id in the sequence
                pFile.write((const char* ) & v, sizeof(uint32_t));
#ifdef CHECK_COLLISION
                // insert the hash and the word to check collisions in the map
                coll_map[hash] = std::move(word);
#else
                coll_map.insert(hash);
#endif
            }else{
                // already seen phrase
#ifdef CHECK_COLLISION
                //check collision
                if(coll_map[hash] != word){
                    word.clear();
                    throw "COLLISION FOUND";
                }
#endif
                word.clear();
                uint32_t v = rank_hash[hash];

#ifdef DEBUG_PRINT

                std::cout<<v<<"- p "<<std::endl;

#endif
                pFile.write((const char* )& v,sizeof(uint32_t));
            }
            results._seq_len++;
        }


        // main function parse the file in conf obj and return and structure
        void parseFile(){
                // open file to read
                std::fstream ffile(config->inputFile(), std::ios::in);
                if (!ffile.is_open()) {
                    std::cout << "Error opening the file: " << config->inputFile() << std::endl;
                    return;
                }
                // we will read max 32 bit Integer
                uint32_t c = 0;
                //current phrase
                std::vector<uint32_t> word;
                // reading file
                while (!ffile.eof() && ffile.read((char *) &c, config->bytesToRead()))
                {
                    //keep max value in alph
                    results._max_alph_val  = std::max(results._max_alph_val,c);
                    // add new element to the current phrase
                    word.push_back(c);
                    // compute new hash for the window
                    hash_type hash  = (uint64_t) config->getWindow()->hashAddCharToWindow(c);
                    //partition condition
                    if (hash % config->mod() == 0 && word.size() >= config->getWindow()->sizeWindow()) {
#ifdef DEBUG_PRINT
                        for(int i:word){
                            std::cout<<(char)i;
                        }
                        std::cout<<std::endl;
#endif
                        addWord(word);
                        config->getWindow()->reset();
                        word.clear();
                    }
                }
                //add possible last phrase in the buffer "word"
                if (word.size()){
                    addWord(word,false);
                    config->getWindow()->reset();
                }
        }

        // Secondary Memory version
        void parseFileSM(){


#ifdef DEBUG_PRINT
            std::cout<<"parseFileSM..................."<<std::endl;
             byte = (config->bytesToRead() == 1);
#endif
            // open file to read
            std::fstream ffile(config->inputFile(), std::ios::in);
            if (!ffile.is_open()) {
                std::cout << "Error opening the file: " << config->inputFile() << std::endl;
                throw "Error opening the file: "+config->inputFile() ;
            }

            std::fstream ffiled(config->inputFile()+".dicc", std::ios::out|std::ios::binary);
            std::fstream ffilep(config->inputFile()+".parse", std::ios::out|std::ios::binary);

            // we will read max 32 bit Integer
            uint32_t c = 0;
            //current phrase
            std::vector<uint32_t> word;

            // reading file
            while (!ffile.eof() && ffile.read((char *) &c, config->bytesToRead()))
            {
                //keep max value in alph
                results._max_alph_val  = std::max(results._max_alph_val,c);
                // add new element to the current phrase
                word.push_back(c);
                // compute new hash for the window
                hash_type hash  = (uint64_t) config->getWindow()->hashAddCharToWindow(c);
                //partition condition
                if (hash % config->mod() == 0 && word.size() >= config->getWindow()->sizeWindow()) {

                    addWord(word,ffiled,ffilep,false);
                    config->getWindow()->reset();
                    word.clear();
                }
            }
            //add possible last phrase in the buffer "word"
            if (!word.empty()){
                addWord(word,ffiled,ffilep,false);
                config->getWindow()->reset();

                word.clear();
            }

            CLogger::GetLogger()->model[config->inputFile()+":dicc-len"] = results._dicc_len;
            CLogger::GetLogger()->model[config->inputFile()+":parse-len"] = results._seq_len;


        }


        void recreateFile(const std::string & s, int bytesToWrite)
        {
            std::fstream ffiled(s+".dicc", std::ios::in|std::ios::binary);
            std::fstream ffilep(s+".parse", std::ios::in |std::ios::binary);
            std::fstream ofile(s+"_recreated", std::ios::out |std::ios::binary);

            uint32_t ch = 0;
            std::unordered_map<uint32_t,std::vector<uint32_t>> dicc;
            uint32_t cont = 1;
            while(!ffiled.eof() && ffiled.read((char*)&ch,sizeof(uint32_t))){
                if(ch == 0){
                    ++cont;
                }else{
                    dicc[cont].emplace_back(ch);
                }
            }

            ch = 0;
            while(!ffilep.eof() && ffilep.read((char*)&ch,sizeof(uint32_t))) {
                auto it = dicc.find(ch);
                if (it == dicc.end()) throw "PHRASE DID NOT FOUND IN THE DICC";

                for (uint32_t c:it->second)
                    ofile.write((char *) &c, bytesToWrite);
            }


        }




    };



}

#endif //RRPAIR_HASHPARSER_H
