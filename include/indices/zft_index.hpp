//
// Created by via56 on 06-12-20.
//

#ifndef RRPAIR_ZFT_INDEX_HPP
#define RRPAIR_ZFT_INDEX_HPP

#include "interface.hpp"

namespace lc {

    class g_z_index:public lcg::g_bs_index {
    public:
        g_z_index() = default;
        ~g_z_index() = default;

        trees::z_fast_trie<std::string,uint64_t , std::unordered_map<std::string,uint64_t,lcg::hash_functions::hMzzStr>> rulesTrie;
        trees::z_fast_trie<std::string,uint64_t , std::unordered_map<std::string,uint64_t,lcg::hash_functions::hMzzStr>> suffixesTrie;
    };


}

#endif //RRPAIR_ZFT_INDEX_HPP
