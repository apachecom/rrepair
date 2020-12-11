//
// Created by via56 on 10-12-20.
//

#ifndef RRPAIR_UTILS_H
#define RRPAIR_UTILS_H

namespace indices {


    namespace utils {


        struct primaryOcc {
            uint64_t preorder_node;
            uint64_t off;

            primaryOcc() = default;

            primaryOcc(const uint64_t &p, const uint64_t &o) : preorder_node(p), off(o) {}
        };


        struct grid_range {
            uint64_t row_1;
            uint64_t row_2;
            uint64_t col_1;
            uint64_t col_2;

            grid_range() = default;

            grid_range(const uint64_t &r1, const uint64_t &r2, const uint64_t &c1, const uint64_t &c2) : row_1(r1),
                                                                                                         row_2(r2),
                                                                                                         col_1(c1),
                                                                                                         col_2(c2) {}
        };


    }

}

#endif //RRPAIR_UTILS_H
