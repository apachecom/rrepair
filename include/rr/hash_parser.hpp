//
// Created by via56 on 30-11-20.
//

#ifndef RRPAIR_HASH_PARSER_HPP
#define RRPAIR_HASH_PARSER_HPP

#include <cstring>
#include <fstream>
#include <unordered_map>
#include <vector>

//#define CHECK_COLLISION

#include "../fingerprints/fingerprints.hpp"
#include "sliding_window.hpp"


namespace rr {

    template <typename hash_type,typename c_type,typename kwindow> class hParser;

//    typedef hParser<uint64_t,unsigned char, rr::KR_window> parserUC32;
//    typedef hParser<uint64_t,unsigned char, rr::w_kr_uc64> parserUC64;
    typedef hParser<uint64_t,unsigned char, rr::KR_window> mzzParserUC64;


    template <typename c_type,typename P >
    void addWord(std::basic_string<c_type>&, std::fstream& dFile, std::fstream& pFile, P&, bool constrain);
    template <typename c_type,typename P >
    void addWord(std::vector<c_type>&, std::fstream& dFile, std::fstream& pFile, P&, bool constrain);

    template <typename P>
    void compress(const std::string& ,P&);
    template <typename P>
    void compress_in_mem(const std::string& ,P&,const uint32_t &);
    template <typename P>
    void decompress(const std::string&,P&);




//
//    template <>
//    void addWord(std::basic_string<unsigned char>&, std::fstream& , std::fstream& , parserUC64&, bool constrain);
//    template <>
//    void compress<parserUC64>(const std::string& ,parserUC64 &);
//    template <>
//    void decompress<parserUC64>(const std::string& ,parserUC64 &);




    template <>
    void addWord(std::basic_string<unsigned char>&, std::fstream& , std::fstream& , mzzParserUC64&, bool constrain);
    template <>
    void compress<mzzParserUC64>(const std::string& ,mzzParserUC64 &);
    template <>
    void decompress<mzzParserUC64>(const std::string& ,mzzParserUC64&);
    template <>
    void compress_in_mem<mzzParserUC64>(const std::string& ,mzzParserUC64&,const uint32_t &);


    template <
            typename hash_type,
            typename c_type,
            typename kwindow
    >
    class hParser{
        public:
            hParser() = default;
            ~hParser() = default;
        uint64_t mod;
        kwindow windows;
        uint64_t diccSize{0};
        std::unordered_map<uint64_t,uint64_t> hashToIds;
#ifdef CHECK_COLLISION
        std::unordered_map<uint64_t,std::basic_string<c_type>> check_collision;
#endif
    };




}

#endif //RRPAIR_HASH_PARSER_HPP
