//
// Created by via56 on 06-12-20.
//

#ifndef RRPAIR_INDICES_HPP
#define RRPAIR_INDICES_HPP

#include "interface.hpp"
#include "../lc/compressed_grammar.hpp"

namespace lc {


    template<
            typename Grid = sdsl::wt_ap<>
            >
    class g_bs_index {
    public:

        g_bs_index() = default;
        ~g_bs_index() = default;

//        lc::lc_grammar<> _g;
//        Grid _grid;
//        std::unordered_map<uint64_t,uint32_t> pi;
//        std::unordered_map<std::string,uint64_t,hash_functions::hMzzStr> fingerprints;

    };
}
#endif //RRPAIR_INDICES_HPP
