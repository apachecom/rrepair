//
// Created by via56 on 30-11-20.
//

#ifndef RRPAIR_RR_HASH_PARSER_HPP
#define RRPAIR_RR_HASH_PARSER_HPP

#include <cstring>
#include <fstream>
#include <unordered_map>



#include "rr_fingerprints.hpp"
#include "rr_sliding_window.hpp"


namespace hash_parser {

    template <typename hash_type,typename c_type,typename kwindow> class hParser;

//    typedef fingerprints::kr_hash<char,uint64_t> kr_hash_c64;
//    typedef fingerprints::kr_hash<unsigned char,uint64_t> kr_hash_uc64;
//    typedef fingerprints::kr_hash<uint32_t ,uint64_t> kr_hash_ui64;

//    typedef parse::krb_fingerprint_windows<unsigned char,uint64_t,kr_hash_uc64,1999999973> w_kr_hash_uc64 ;

    // parser para unsigned char y hash 64 bits
    typedef hParser<uint64_t,unsigned char, parse::w_kr_uc64> parserUC64;


    template <typename c_type,typename P >
    void addWord(std::basic_string<c_type>&, std::fstream& dFile, std::fstream& pFile, P&);

    template <typename P>
    void compress(const std::string& ,P&);

    template <typename P>
    void decompress(const std::string&,P&);





    template <>
    void addWord(std::basic_string<unsigned char>&, std::fstream& , std::fstream& , parserUC64&);
    template <>
    void compress<parserUC64>(const std::string& ,parserUC64 &);
    template <>
    void decompress<parserUC64>(const std::string& ,parserUC64 &);



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
    };




}

#endif //RRPAIR_RR_HASH_PARSER_HPP
