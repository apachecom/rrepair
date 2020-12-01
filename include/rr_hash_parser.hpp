//
// Created by via56 on 30-11-20.
//

#ifndef RRPAIR_RR_HASH_PARSER_HPP
#define RRPAIR_RR_HASH_PARSER_HPP

#include <cstring>
#include <fstream>

namespace hash_parser {


    template <typename c_type,typename P >
    void addWord(const std::basic_string<c_type>&, std::fstream& dFile, std::fstream& pFile, P&);

    template <typename c_type, typename P>
    void parseFile(const std::string file,P&);


    template <
            typename hash_type,
            typename c_type,
            typename hash_function
    >
    class hParser{
        public:
            hParser() = default;
            ~hParser() = default;
            hash_function hash; // std::basic_string<c_type> - hash_type
    };

    template <>
    class hParser<uint64_t,>{
    public:
        hParser() = default;
        ~hParser() = default;
        hash_function hash; // std::basic_string<c_type> - hash_type
    };



}

#endif //RRPAIR_RR_HASH_PARSER_HPP
