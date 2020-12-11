//
// Created by via56 on 10-12-20.
//

#ifndef RRPAIR_UTILS_HPP
#define RRPAIR_UTILS_HPP



namespace grid {

    namespace utils {

        template<D,L>
        struct point{
            T row{0},col{0};
            L value{0};
        };

        template<D>
        struct query{
            T row_1{0},col_1{0},row_2{0},col_2{0};
        };

    }

}

#endif //RRPAIR_UTILS_HPP
