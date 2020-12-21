//
// Created by via56 on 01-12-20.
//

#include "../../include/rr/hash_parser.hpp"

#include <iostream>
#include <vector>


using namespace fingerprints;
using namespace rr;

template <>
void rr::addWord(std::vector<uint8_t>& word, std::fstream& dFile, std::fstream& pFile, mzzParserUC64& parser,bool constrain){

    unsigned char zero = 0;
    word.push_back(zero);
    uint len = word.size();
    //check the word is big enough
    if (len < rr::size_window(parser.windows) && constrain){
        std::cout<<"windows-size:"<<rr::size_window(parser.windows)<<std::endl;
        std::cout<<"word-len:"<<len<<std::endl;
        throw "windows to small";
    }

    const char* ptr = (const char*) word.data();
    // compute the hash of the string
    uint64_t hash = parser.windows.fhash.hash(ptr);
    //search the has in the dicc
#ifdef CHECK_COLLISION
    auto it_cc = parser.check_collision.find(hash);
    if(it_cc == parser.check_collision.end()){
        parser.check_collision[hash] = word;
    }else{
        if(it_cc->second != word)
            throw "COLLISION FOUND";
    }
#endif
    auto it = parser.hashToIds.find(hash);
    if(it == parser.hashToIds.end()){
        ++parser.results._dicc_len;
        //new phrase...

#ifdef RR_DEBUG_PRINT

        std::cout<<"dicc["<<parser.results._dicc_len<<"]:";
        for (const auto &item : word)
            std::cout<<item<<" ";
        std::cout<<std::endl;
#endif

        dFile.write(ptr,word.size()*sizeof(unsigned char));
        parser.hashToIds[hash] = ++(parser.diccSize);
        uint_t_write id_phrase = parser.diccSize;
        pFile.write((const char* ) & id_phrase, sizeof(uint_t_write));
    }else{
        // already existing phrase...
        uint_t_write id_phrase = it->second;


#ifdef RR_DEBUG_PRINT

        std::cout<<"parse["<<parser.results._seq_len+1<<"]:"<<id_phrase<<std::endl;

#endif

        pFile.write((const char* ) & id_phrase, sizeof(uint_t_write));
    }

    parser.results._seq_len++;
    word.clear();
}

template <>
void rr::compress_in_mem<mzzParserUC64>(const std::string& file ,mzzParserUC64 & parser, const uint32_t &buffer_size){

//    std::cout<<"compress_in_mem"<<std::endl;

    std::streampos fsize = 0;
    std::fstream ffile(file, std::ios::in);

    fsize = ffile.tellg();
    ffile.seekg( 0, std::ios::end );
    fsize = ffile.tellg() - fsize;

    ffile.seekg( 0, std::ios::beg );
    ffile.clear();
    auto file_size = (uint32_t)fsize;
//    std::cout<<"file eof:"<<ffile.eof()<<std::endl;
//    std::cout<<"file size:"<<file_size<<std::endl;
//

    if (!ffile.is_open()) {
        std::cout << "Error opening the file: " << file << std::endl;
        throw "Error opening the file: "+ file ;
    }
    std::fstream ffiled(file+".dicc", std::ios::out|std::ios::binary);
    std::fstream ffilep(file+".parse", std::ios::out|std::ios::binary);


    //current phrase
    std::vector<uint8_t> word;
    // reading file
    uint64_t wsize = rr::size_window(parser.windows);
    // we will read byte by byte Integer
    uint8_t *c = new uint8_t [buffer_size];
    uint32_t bytes = parser.windows.bytexsymb;

    uint32_t p = file_size/buffer_size;

//    std::cout<<"partition file:"<<p<<std::endl;

    for(uint32_t j = 0 ; j < p; j++){

//        std::cout<<" \tpartition:"<<j<<std::endl;
        ffile.read((char*) c, buffer_size);

        for (uint32_t i = 0; i < buffer_size; i+=bytes) {
//                std::cout<<" \tc:"<<i<<"\r";
                append(word,&c[i],bytes);
//                word.push_back(c[i]);
                // compute new hash for the window
                uint64_t hash  = rr::feed(&c[i],parser.windows);
                //partition condition
                if (hash % parser.mod == 0 && word.size() >= wsize) {
                    rr::addWord(word,ffiled,ffilep,parser,true);
                    rr::reset(parser.windows);
                    word.clear();
                }
            }
    }

    delete c;
    uint32_t rest = file_size%buffer_size;

//    std::cout<<"rest of file:"<<rest<<std::endl;
    c = new uint8_t[rest];
    if(rest > 0){

        ffile.read((char*) c, rest);

        for (uint32_t i = 0; i < rest; ++i) {

            append(word,&c[i],bytes);
            // compute new hash for the window
            uint64_t hash  = rr::feed(&c[i],parser.windows);
            //partition condition
            if (hash % parser.mod == 0 && word.size() >= wsize) {
                rr::addWord(word,ffiled,ffilep,parser,true);
                rr::reset(parser.windows);
                word.clear();
            }
        }
    }

//    std::cout<<std::endl;
    //add possible last phrase in the buffer "word"
    if (!word.empty()){
        rr::addWord(word,ffiled,ffilep,parser,false);
        rr::reset(parser.windows);
        word.clear();
    }

    delete c;

}

template <>
void rr::compress<mzzParserUC64>(const std::string& file ,mzzParserUC64 & parser){

    std::fstream ffile(file, std::ios::in);
    if (!ffile.is_open()) {
        std::cout << "Error opening the file: " << file << std::endl;
        throw "Error opening the file: "+ file ;
    }
    std::fstream ffiled(file+".dicc", std::ios::out|std::ios::binary);
    std::fstream ffilep(file+".parse", std::ios::out|std::ios::binary);


    uint64_t file_size = io::getFileSize(ffile);
    uint32_t bytes = parser.windows.bytexsymb;
    // we will read byte by byte Integer
    uint8_t * c = new uint8_t [bytes];
    //current phrase
    std::vector<uint8_t> word;
    // reading file
    uint64_t wsize = rr::size_window(parser.windows);


    uint64_t it = file_size/bytes;
    for (uint64_t i = 0; i < it ; ++i) {
        ffile.read((char*) c, bytes);
        append(word,c,bytes);

        uint64_t * ptr = new uint64_t; *ptr = 0;
        uint8_t * tmp  = (uint8_t*)ptr ;
        for (int j = 0; j < bytes ; ++j) tmp[j] = c[j];
        if(*ptr > parser.results._max_alph_val)
            parser.results._max_alph_val = *ptr;

        delete ptr;

        // compute new hash for the window
        uint64_t hash  = rr::feed(c,parser.windows);
        //partition condition
        if (hash % parser.mod == 0 && word.size() >= wsize) {
            ++parser.results._seq_len;
            rr::addWord(word,ffiled,ffilep,parser,true);
            rr::reset(parser.windows);
            word.clear();
        }
    }
    delete [] c;
    it = file_size%bytes;
    parser.windows.bytexsymb = 1;
    bytes = 1;
    c = new uint8_t [bytes];

    for (uint64_t i = 0; i < it ; ++i) {
        ffile.read((char*) c, bytes);
        append(word,c,bytes);
        // compute new hash for the window
        uint64_t hash  = rr::feed(c,parser.windows);
        //partition condition
        if (hash % parser.mod == 0 && word.size() >= wsize) {
            ++parser.results._seq_len;
            rr::addWord(word,ffiled,ffilep,parser,true);
            rr::reset(parser.windows);
            word.clear();
        }
    }


//    while (!ffile.eof() && ffile.read((char*) c, bytes))
//    {   // add new element to the current phrase
//        append(word,c,bytes);
//        // compute new hash for the window
//        uint64_t hash  = rr::feed(c,parser.windows);
//        //partition condition
//        if (hash % parser.mod == 0 && word.size() >= wsize) {
//            rr::addWord(word,ffiled,ffilep,parser,true);
//            rr::reset(parser.windows);
//            word.clear();
//        }
//    }
    //add possible last phrase in the buffer "word"
    if (!word.empty()){
        rr::addWord(word,ffiled,ffilep,parser,false);
        rr::reset(parser.windows);
        word.clear();
    }


    delete [] c;

#ifdef RR_DEBUG_PRINT
    std::cout<<"hash-parser:_max_alph_val:"<<parser.results._max_alph_val<<std::endl;
#endif

}

template <>
void rr::decompress<mzzParserUC64>(const std::string& file,mzzParserUC64 & parser){

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



