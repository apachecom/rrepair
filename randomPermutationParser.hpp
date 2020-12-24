//
// Created by via56 on 22-12-20.
//

#ifndef RRPAIR_RANDOMPERMUTATIONPARSER_HPP
#define RRPAIR_RANDOMPERMUTATIONPARSER_HPP

#include <list>
#include <cassert>
#include <unordered_map>
#include <random>
#include <algorithm>
#include "RepairUtils.h"
#include "CLogger.h"


#define UP 0
#define DOWN 1
#define MAX_BUFFER_SIZE 1000000

using namespace big_repair;


namespace std {

    template<>
    struct hash<std::vector<uint32_t>> {

        std::size_t operator()(const std::vector<uint32_t>& str) const noexcept {

            uint64_t hash = 0;
            //const uint64_t prime = 3355443229;     // next prime(2**31+2**30+2**27)
            const uint64_t prime = 27162335252586509; // next prime (2**54 + 2**53 + 2**47 + 2**13)
            auto ptr = (unsigned char* )str.data();
            uint32_t len = str.size()*sizeof (uint32_t);
            for (size_t k = 0; k < len; k++) {
                int c = (unsigned char) ptr[k];
                assert(c >= 0 && c < 256);
                hash = (256 * hash + c) % prime;    //  add char k
            }
            return (size_t) hash;
        }

    };


}

class RandomPermutationParser {


public:
    RandomPermutationParser() = default;


    virtual ~RandomPermutationParser() {
        if(pi != nullptr) delete [] pi;
        pi = nullptr;
    }


    struct stats{
        std::string filename;
        uint32_t dicc_len{0};
        uint32_t seq_len{0};
        uint32_t max_sigma{0};

        void reset(){

            dicc_len  = 0;
            seq_len   = 0;
            max_sigma = 0;

        }
    };

//    template<uint64_t buffer_size = MAX_BUFFER_SIZE >
//    struct file_manager{
//
//        uint32_t alph_max{0};
//        uint32_t file_len{0};
//        unsigned char * buffer{nullptr};
//        std::ifstream in;
//
//        file_manager(const std::string file) {
//            in.open(file, std::ifstream ::in);
//            in.seekg(0,std::ios_base::end);
//            file_len = in.tellg();
//            in.seekg(0,std::ios_base::beg);
//        }
//
//        read(unsigned char**)
//
//
//    };

    stats params;
    std::unordered_map<std::vector<uint32_t>,uint64_t> hash_table;
    uint32_t* pi{nullptr};



    void generate_permutation(const uint& len){

            if(pi != nullptr) delete [] pi;

            pi = new uint32_t [len];
            pi[0] = 0;
            for(size_t i = 1; i < len; i++){
                pi[i] = i;
            }

            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(pi + 1,pi + len,g);
//
//        for (uint32_t j = 0 ; j < len; ++j) {
//            std::cout<<"pi["<<j<<"]:"<<pi[j]<<std::endl;
//        }
//        std::cout<<std::endl;

    }


    void parser (const std::string &file, const uint32_t& sigma, const uint& bytex){

        params.filename = file;

        generate_permutation(sigma);

        std::fstream ffile(params.filename, std::ios::in);
        if (!ffile.is_open()) {
            std::cout << "Error opening the file: " << params.filename << std::endl;
            throw "Error opening the file: "+ params.filename ;
        }

        std::fstream ffiled(params.filename+".dicc", std::ios::out|std::ios::binary);
        std::fstream ffilep(params.filename+".parse", std::ios::out|std::ios::binary);

        // we will read max 32 bit Integer
        uint32_t c = 0;
        //current phrase
        std::vector<uint32_t> word;
        // reading file
        int state = UP; uint32_t last = 0;

        while (!ffile.eof() && ffile.read((char *) &c, bytex)) {

            params.max_sigma = params.max_sigma > c ? params.max_sigma : c;

            if(state == DOWN && pi[c] > pi[last]){

//                // mark the phrase
//                std::cout<<"["<<hash_table.size() + 1<<"]"<<std::endl;
//                for (const auto &item : word) {
//                    std::cout<<item<<" ";
//                }
//                std::cout<<std::endl;
//
//                for (const auto &item : word) {
//                    std::cout<<pi[item]<<" ";
//                }
//                std::cout<<"*******************"<<std::endl;

                addWord(word,ffiled,ffilep,false);
                state = UP;
            }else {
                if(pi[c] > pi[last]){
                    state = UP;
                }else{
                    if(pi[c] < pi[last])
                        state = DOWN;
                }
            }
            word.push_back(c);
            last = c;

        }
        if (!word.empty()){
            addWord(word,ffiled,ffilep,false);
        }



        CLogger::GetLogger()->model[file+":dicc-len"] = params.dicc_len;
        CLogger::GetLogger()->model[file+":parse-len"] = params.seq_len;

    }


    void addWord(std::vector<uint32_t>& word, std::fstream& dFile, std::fstream& pFile, bool constrain = true){

        uint len = word.size();
        //search the has in the dicc
        auto it = hash_table.find(word);

        if (it == hash_table.end()) {
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

            params.dicc_len++;
            //write the phrase in the dictionary file
            auto data = (char*) word.data();
            dFile.write((char *) data,word.size() * sizeof(uint32_t));
            //put 0 between phrases
            uint32_t z = 0;
            dFile.write((const char *)&z,sizeof(uint32_t));

            uint32_t d = hash_table.size() + 1 ;
            // write the id in the sequence
            pFile.write((const char* ) & d, sizeof(uint32_t));
#ifdef CHECK_COLLISION
            // insert the hash and the word to check collisions in the map
                coll_map[hash] = std::move(word);
#else
            hash_table[word] = d;
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
            uint32_t v = it->second;

#ifdef DEBUG_PRINT

            std::cout<<v<<"- p "<<std::endl;

#endif
            pFile.write((const char* )& v,sizeof(uint32_t));
        }

        word.clear();
        params.seq_len++;
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


#endif //RRPAIR_RANDOMPERMUTATIONPARSER_HPP