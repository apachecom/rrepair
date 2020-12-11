//
// Created by via56 on 06-12-20.
//

#ifndef RRPAIR_INTERFACE_HPP
#define RRPAIR_INTERFACE_HPP


#include <cstdint>
#include <string>
#include <set>
#include <vector>
#include "utils.hpp"

namespace indices {

    using namespace utils;

    template<typename I>
    uint64_t sizeInBytes(I *indice);

    template<typename I>
    uint64_t build(I *indice);

    template<typename I>
    uint64_t destroy(I *indice);


    template<typename I>
    std::string extraction(const uint64_t &i, const uint64_t &len, I *indice);

    template<typename I>
    void locate(const char *, const uint64_t &len, I *indice, std::set <uint64_t> &);

    template<typename I>
    uint64_t count(const uint64_t &i, const uint64_t &len, I *indice);

    template<typename I>
    void gridSearch(const grid_range &, const uint64_t &q, std::vector<primaryOcc> &, I *indice);

    template<typename I>
    void findPartition(const char *, const uint64_t &len, std::set <uint32_t> &, I *indice);

    // return true if found a posible range and return the grid range
    template<typename I>
    bool findGridRanges(const char *, const uint64_t &len, const uint64_t &q,
                        grid_range &, I *indice);

    template<typename I>
    void findSecondaryOcc(const primaryOcc &, const uint64_t &, std::set <uint64_t> &pos, I *);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template<typename I>
    void locate(const char *pattern, const uint64_t &len, const I *indice, std::set <uint64_t> &pos) {
        std::set <uint32_t> partitions;
        findPartition(pattern, len, partitions, indice);
        for (const auto &item : partitions) {
            //find primary occ
            grid_range range;
            //range search
            if (findGridRanges(pattern, len, item, range, indice)) {
                std::vector <primaryOcc> pOcc;
                // grid search
                gridSearch(range, item, pOcc, &indice);
                // find secondary occ
                for (const auto &occ : pOcc) {
                    findSecondaryOcc(occ, pos, indice);
                }
            }

        }
    }
}

#endif //RRPAIR_INTERFACE_HPP
