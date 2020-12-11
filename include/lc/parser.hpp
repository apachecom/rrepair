//
// Created by via56 on 05-12-20.
//

#ifndef RRPAIR_PARSER_HPP
#define RRPAIR_PARSER_HPP



#include <unordered_map>
#include <sdsl/bit_vectors.hpp>

#include "../fingerprints/fingerprints.hpp"
#include "../logger/CLogger.h"
#include "../../include/utils/io.hpp"

#include "symbols.hpp"
#include "util.hpp"
#include "macros.hpp"

#define ZERO_SYM 1

namespace lc {

#ifdef PRINT_LOGS
        template<typename P>
    void print_parser(const P&);
#endif
        template<typename P>
        bool check_expansion(const uint64_t & i ,const uint64_t & j,std::string &, const P& );
        template <typename P>
        void parse_text(const char* _text, const uint64_t & _n_text, P& );
        template <typename P>
        void parse_file(const std::string &file, P& );
        template <typename P>
        void runs( std::vector<typename P::rule_type*>& MT, std::vector<uint64_t> &POS, P& );
        template <typename P>
        void blocks( std::vector<typename P::rule_type*>& MT, std::vector<uint64_t> &POS, P& );
        template <typename P>
        void decompress( std::fstream& , P& );
        template <typename P>
        void decompress( std::string & , P& );
        template <typename P>
        void destroy(P& );

        template <typename hFunction = fingerprints::MzzHash>
        class parser {

            public:
                typedef uint64_t hash_type ;
                typedef std::unordered_map<std::string,MetaSymbol*,hFunction> HashTableRule;

                typedef MetaSymbol rule_type;
                typedef lc::MSrun run_type;
                typedef lc::MSblock block_type;

                char * text{nullptr}; // original text
                uint64_t n_text;
                HashTableRule hash_rules;
                hFunction kp_f;
                uint64_t n_sigma{0}; // id incremental
                uint64_t n_pi{0}; // pi incremental
                rule_type* S{nullptr};
                std::vector<char> alph;
        };
    }



#endif //RRPAIR_PARSER_HPP
