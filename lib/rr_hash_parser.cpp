//
// Created by via56 on 01-12-20.
//

#include "../include/rr_hash_parser.hpp"
#include <iostream>


using namespace fingerprints;
using namespace hash_parser;




// parser para unsigned char y hash 64 bits
typedef hParser<uint64_t,unsigned char, kr_hash_uc64> parserUC64;

template <>
void hash_parser::addWord(std::basic_string<unsigned char>&, std::fstream& , std::fstream& , parserUC64&);

template <>
void hash_parser::compress<parserUC64>(const std::string& ,parserUC64 &);

template <>
void hash_parser::decompress<parserUC64>(const std::string& ,parserUC64 &);



template <>
void hash_parser::addWord(std::basic_string<unsigned char>& word, std::fstream& dFile, std::fstream& pFile, parserUC64& parser
#ifdef MEM_MONITOR
#endif
){

    unsigned char zero = 0;
    word.push_back(zero);
    uint len = word.size();
    //check the word is big enough
    if (len < parse::size_window(parser.windows) ){
        throw "windows to small";
    }

    const char* ptr = (const char*) word.data();
    // compute the hash of the string
    uint64_t hash = parser.windows.hash(word);
    //search the has in the dicc

#ifdef CHECK_COLLISIONS
#else
    auto it = parser.hashToIds.find(hash);
    if(it == parser.hashToIds.end()){
        //new phrase...
        dFile.write(ptr,word.size()*sizeof(unsigned char));
        parser.hashToIds[hash] = ++(parser.diccSize);
        pFile.write((const char* ) & parser.diccSize, sizeof(uint64_t));
    }else{
        // already existing phrase...
        uint64_t p = it->second;
        pFile.write((const char* ) & p, sizeof(uint64_t));
    }
#endif

    word.clear();
}

template <>
void hash_parser::compress<parserUC64>(const std::string& file ,parserUC64 & parser){

    std::fstream ffile(file, std::ios::in);
    if (!ffile.is_open()) {
        std::cout << "Error opening the file: " << file << std::endl;
        throw "Error opening the file: "+ file ;
    }
    std::fstream ffiled(file+".dicc", std::ios::out|std::ios::binary);
    std::fstream ffilep(file+".parse", std::ios::out|std::ios::binary);
    // we will read byte by byte Integer
    unsigned char c = 0;
    //current phrase
    std::basic_string<unsigned char> word;
    // reading file
    uint64_t wsize = parse::size_window(parser.windows);
    while (!ffile.eof() && ffile.read((char*) &c, 1))
    {   // add new element to the current phrase
        word.push_back(c);
        // compute new hash for the window
        uint64_t hash  = parse::feed(c,parser.windows);
        //partition condition
        if (hash % parser.mod == 0 && word.size() >= wsize) {
            hash_parser::addWord(word,ffiled,ffilep,parser);
            parse::reset(parser.windows);
            word.clear();
        }
    }
    //add possible last phrase in the buffer "word"
    if (!word.empty()){
        hash_parser::addWord(word,ffiled,ffilep,parser);
        parse::reset(parser.windows);
        word.clear();
    }
}

template <>
void hash_parser::decompress<parserUC64>(const std::string& file,parserUC64 & parser){

    std::fstream ffiled(file+".dicc", std::ios::in|std::ios::binary);
    std::fstream ffilep(file+".parse", std::ios::in |std::ios::binary);
    std::fstream ofile(file+".out", std::ios::out |std::ios::binary);

    unsigned char ch = 0;
    std::unordered_map<uint64_t ,std::basic_string<unsigned char>> dicc;
    uint64_t cont = 1;
    while(!ffiled.eof() && ffiled.read((char*)&ch,1)){
        if(ch == 0){
#ifdef DEBUG_PRINT
            std::cout<<":"<<cont<<std::endl;
#endif
            ++cont;
        }else{
            dicc[cont].push_back(ch);
#ifdef DEBUG_PRINT
            std::cout<<ch<<",";
#endif
        }
    }
#ifdef DEBUG_PRINT
    std::cout<<"sequence:"<<std::endl;
#endif
    uint64_t p = 0;
    while(!ffilep.eof() && ffilep.read((char*)&p,sizeof(uint64_t))) {
#ifdef DEBUG_PRINT
        std::cout<<ch<<std::endl;
#endif
        auto it = dicc.find(p);
        if (it == dicc.end()) {
            std::cout<<"PHRASE DID NOT FOUND IN THE DICC "<<ch<<std::endl;
            throw "PHRASE DID NOT FOUND IN THE DICC ";

        }
        for (auto c:it->second)
            ofile.write((char *) &c, 1);
    }


}

