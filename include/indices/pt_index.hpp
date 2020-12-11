//
// Created by via56 on 06-12-20.
//

#ifndef RRPAIR_PT_INDEX_HPP
#define RRPAIR_PT_INDEX_HPP

#include "interface.hpp"
namespace lc {

    class g_pt_index:public lcg::g_bs_index {
    public:

        g_pt_index() = default;
        ~g_pt_index() = default;

        trees::compact_patricia_tree rulesTrie;
        trees::compact_patricia_tree suffixesTrie;

    };

}


#endif //RRPAIR_PT_INDEX_HPP
