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
#include "RepairUtils.h"
#include "include/rr/rr_trie.hpp"

//#define DEBUG_PRINT 1
#define CHECK_COLLISION

namespace big_repair{

/*
 * This class read a file n-byte by n-bytes and create a dictionary of symbols and the sequence corresponding to the file
 */

    template <
            typename C,     //Config object
            typename hash_type = uint_t
    >
    class HashParser {

    public:
        //todo ver como implementar stats!!
        struct stats{
            uint_t _max_alph_val;
            uint_t _seq_len;
            uint_t _dicc_len;
            uint_t _dicc_len_size;
            uint_t _check_size;
            uint_t _collision_times;
            std::vector<uint_t> seq_len_size;


            stats(){
                _max_alph_val = 0;
                _seq_len = 0;
                _dicc_len = 0;
                _check_size = 0;
                _dicc_len_size = 0;
                _collision_times = 0;
            }
            void print(){


//#ifdef DEBUG_PRINT
                std::cout<<"-------------------Parser Data------------------------------\n";
                std::cout<<"Max value in alph:" << _max_alph_val << std::endl;
                std::cout<<"Initial Sequence len:" << _seq_len << std::endl;
                std::cout<<"Dicctionary len:" << _dicc_len << std::endl;
                std::cout<<"Dicctionary size:" << _dicc_len_size << std::endl;
                std::cout<<"Check dicc vs File size:" << _check_size << std::endl;
                uint_t acc = 0;

                for (const auto &item : seq_len_size)
                    acc += item;
                std::cout<<"Check seq vs File size:" << acc << std::endl;

//#endif

            }

        };
        stats results;

    private:
        C* config;

        std::unordered_map<hash_type,tree::ui32_trie32> coll_map;
        std::unordered_map<hash_type,std::unordered_map<uint32_t,uint_t>> node_map_phrase;
        uint_t n_phrases{0};
//        std::vector<uint_t> compressed_seq;

#ifdef DEBUG_PRINT
    bool byte;
#endif


    public:

        HashParser() {
            config = nullptr;
            n_phrases = 0;
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
            for (auto &item : coll_map) {
                tree::destroy(item.second);
            }
        }

        void addWord(std::vector<uint_t>& word, std::fstream& dFile, std::fstream& pFile, bool constrain = true){
            uint_t zero = 0;
            word.push_back(zero);
            uint len = word.size();

            //check the word is big enough
            if (len < config->getWindow()->sizeWindow() && constrain){
                throw "windows to small";
            }

            const char* ptr = (const char*) word.data();
            // compute the hash of the string
            hash_type hash = config->hashFunction()->apply(word,word.size());
            //search the has in the dicc
            auto it = coll_map.find(hash);
            results.seq_len_size.push_back(word.size());

            if (it == coll_map.end()) {

#ifdef DEBUG_PRINT
                std::cout<<"- new-hash "<<hash<<std::endl;
#endif
                // new phrase
                results._dicc_len_size += word.size();
                results._dicc_len++;
                //write the phrase in the dictionary file
                //put 0 at the end of each phrase

                    dFile.write(ptr,word.size()*sizeof(uint_t));

                // insert the hash and the word to check collisions in the map
                // we store a trie by each hash to check collision
                // we associated the id of the phrase to the leaves of the trie
                tree::ui32_trie32 _trie;

                auto node = tree::init<tree::ui32_trie32,uint32_t,uint32_t>(_trie);
                for (uint32_t i = 0; i < word.size(); ++i) {
                    node = tree::insert(node->id,word[i],_trie);
                }
                coll_map[hash] = _trie;
                node_map_phrase[hash][node->id] = n_phrases+1;
                ++n_phrases;
                pFile.write((const char* ) & n_phrases, sizeof(uint_t));
#ifdef DEBUG_PRINT
                std::cout<<n_phrases<<"- new-p "<<node->id<<" str:";
                for (int i = 0; i < word.size() ; ++i) {
                    std::cout<<word[i]<<",";
                }
                std::cout<<std::endl;
#endif

            }else{
                // already seen phrase
                // check collision
#ifdef DEBUG_PRINT
                std::cout<<"- known-hash "<<hash<<std::endl;
#endif

                auto node = tree::root<tree::ui32_trie32,uint32_t,uint32_t>(coll_map[hash]);
                uint32_t j = 0, l = word.size();
                while( j < l ){
                    auto t_node = tree::feed<tree::ui32_trie32,uint32_t,uint32_t>(node->id,word[j],coll_map[hash]);
                    if(t_node == nullptr) break;
                    node = t_node;
                    ++j;
                }
                if(j != l || !tree::isLeaf<tree::ui32_trie32,uint32_t,uint32_t>(node->id,coll_map[hash])){
//                    not found case
//                    new phrase with equal hash

                    //write the phrase in the dictionary file
                    std::cout<<"collisions:"<<results._collision_times+1<<"\r";
                    results._collision_times++;
                    dFile.write(ptr,word.size()*sizeof(uint_t));
                    //update trie
                    for (int i = j; i < l ; ++i) {
                        node = tree::insert(node->id,word[i],coll_map[hash]);
                    }
                    node_map_phrase[hash][node->id] = n_phrases;
                    ++n_phrases;
                    pFile.write((const char* )& n_phrases,sizeof(uint_t));

#ifdef DEBUG_PRINT
                    std::cout<<n_phrases<<"- new-p "<<node->id<<" str:";
                    for (int i = 0; i < word.size() ; ++i) {
                        std::cout<<word[i]<<",";
                    }
                    std::cout<<std::endl;
#endif
                }else{
                    uint_t p  = node_map_phrase[hash][node->id];
                    pFile.write((const char* )& p,sizeof(uint_t));

#ifdef DEBUG_PRINT
                    std::cout<<p<<"- found-p "<<node->id<<" str:";
                    for (int i = 0; i < word.size() ; ++i) {
                        std::cout<<char(word[i]);
                    }
                    std::cout<<std::endl;
#endif
                }
                word.clear();
            }

            results._seq_len++;
        }

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
            uint_t c = 0;
            //current phrase
            std::vector<uint_t> word;

            // reading file
            while (!ffile.eof() && ffile.read((char *) &c, config->bytesToRead()))
            {
                //keep max value in alph
                results._max_alph_val  = std::max(results._max_alph_val,c);
                // add new element to the current phrase
                word.push_back(c);
                // compute new hash for the window
                hash_type hash  = (uint_t) config->getWindow()->hashAddCharToWindow(c);
                //partition condition
                if (hash % config->mod() == 0 && word.size() >= config->getWindow()->sizeWindow()) {
                    results._check_size += word.size();
                    addWord(word,ffiled,ffilep,false);
                    config->getWindow()->reset();
                    word.clear();
                }
            }
            //add possible last phrase in the buffer "word"
            if (!word.empty()){

                results._check_size += word.size();
                addWord(word,ffiled,ffilep,false);
                config->getWindow()->reset();

                word.clear();
            }

        }


        void recreateFile(const std::string & s,const std::string & out, int bytesToWrite)
        {

            std::fstream ffiled(config->inputFile()+".dicc", std::ios::in|std::ios::binary);
            std::fstream ffilep(config->inputFile()+".parse", std::ios::in |std::ios::binary);
            std::fstream ofile(config->inputFile()+".out", std::ios::out |std::ios::binary);

            uint_t ch = 0;
            std::map<uint_t,std::vector<uint_t>> dicc;
            uint_t cont = 1;
            while(!ffiled.eof() && ffiled.read((char*)&ch,sizeof(uint_t))){

                if(ch == 0){

#ifdef DEBUG_PRINT
                    std::cout<<":"<<cont<<std::endl;
#endif
                    ++cont;
                }else{
                    dicc[cont].emplace_back(ch);

#ifdef DEBUG_PRINT
    std::cout<<ch<<",";
#endif
                }
            }
//
//#ifdef DEBUG_PRINT
//
//            std::cout<<"dicc:"<<std::endl;
//            for (const auto &item : dicc) {
//                std::cout<<item.first<<" : ";
//                for (const auto &item2 : item.second) {
//                    std::cout<<char(item2);
//                }
//                std::cout<<std::endl;
//            }
//#endif
            ch = 0;

#ifdef DEBUG_PRINT
    std::cout<<"sequence:"<<std::endl;
#endif
            while(!ffilep.eof() && ffilep.read((char*)&ch,sizeof(uint_t))) {

#ifdef DEBUG_PRINT
    std::cout<<ch<<std::endl;
#endif
                auto it = dicc.find(ch);
                if (it == dicc.end()) {
                    std::cout<<"PHRASE DID NOT FOUND IN THE DICC "<<ch<<std::endl;
                    throw "PHRASE DID NOT FOUND IN THE DICC ";

                }
                for (uint_t c:it->second)
                    ofile.write((char *) &c, bytesToWrite);
            }
        }
    };



}

#endif //RRPAIR_HASHPARSER_H
