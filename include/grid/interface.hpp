//
// Created by via56 on 10-12-20.
//

#ifndef RRPAIR_INTERFACE_HPP
#define RRPAIR_INTERFACE_HPP

#include "utils.hpp"

namespace grid {

    using namespace utils;

    template<G,D,L>
    void build(std::vector<point<T,D,L>>& points, const D& n_rows,const D& n_cols, *G);

    template<G,D,L>
    void range(query<D>& q, std::vector<L>& values,*G);

    template<G>
    void save(*G);

    template<G>
    void load(*G);

    template<G>
    uint64_t size_in_bytes(*G);


#ifdef PRINT_LOGS
    // here log functions
#endif


}







#endif //RRPAIR_INTERFACE_HPP
