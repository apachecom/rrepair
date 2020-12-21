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
#include "macros.hpp"
#include "../../include/utils/io.hpp"

namespace rr {
    typedef uint64_t uint_t;
    template <typename hash_type,typename c_type,typename kwindow> class hParser;

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
            hParser(const hParser& H ) {
                mod = H.mod;
                windows = H.windows;
                diccSize = H.diccSize;
                results = H.results;
                hashToIds = H.hashToIds;
#ifdef CHECK_COLLISION
                check_collision = H.check_collision;
#endif
            }

            hParser& operator = (const hParser& H ){
                mod = H.mod;
                windows = H.windows;
                diccSize = H.diccSize;
                results = H.results;
                hashToIds = H.hashToIds;
#ifdef CHECK_COLLISION
                check_collision = H.check_collision;
#endif
                return *this;
            }
        uint64_t mod;
        kwindow windows;
        uint64_t diccSize{0};
        std::unordered_map<uint64_t,uint64_t> hashToIds;
#ifdef CHECK_COLLISION
        std::unordered_map<uint64_t,std::basic_string<c_type>> check_collision;
#endif
        struct stats{
            uint_t _max_alph_val;
            uint_t _seq_len;
            uint_t _dicc_len;
            uint_t _dicc_len_size;

//            uint_t _check_size;
//            uint_t _collision_times;
//            std::vector<uint_t> seq_len_size;

            stats(){
                _max_alph_val = 0;
                _seq_len = 0;
                _dicc_len = 0;
//                _check_size = 0;
                _dicc_len_size = 0;
//                _collision_times = 0;
            }

            stats(const stats&) = default;
            stats& operator = (const stats&) = default;

            void print(){


#ifdef DEBUG_PRINT
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

#endif

            }

        };
        stats results;

    };




}

#endif //RRPAIR_HASH_PARSER_HPP
