//
// Created by via56 on 29-11-20.
//

#ifndef RRPAIR_UTILS_HPP
#define RRPAIR_UTILS_HPP


#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <functional>
#include "macros.hpp"
#include <cassert>
namespace rr{



    void append(std::vector<uint8_t>& dest,const std::vector<uint8_t>& src, const uint32_t& n);
    void append(std::vector<uint8_t>& dest,const uint8_t * src, const uint32_t& n);

    std::pair<uint64_t,uint64_t> prepareDiccFileForRP(const std::string &dFile, const std::string &dout, int bytes, uint64_t max_value);

// Return the greater prime less than limit
    template<typename uint_t = uint64_t >
    uint_t computeLowerPrime(const uint_t& Limit ){

        bool * cribe = new bool[Limit+1];

        for (uint_t i = 0; i < Limit + 1; ++i) {
            cribe[i] = 1;
        }

        cribe[2] = 1;

        uint_t g_prime = 0;

        for (uint_t i = 2; i < Limit + 1; ++i) {
            if(cribe[i]){
                g_prime = i;
                for (uint_t j = 2; j*i < Limit + 1  ; ++j)
                    cribe[j*i] = 0;

            }
        }

        delete cribe;
        return g_prime;
    }


// power modulo prime
    uint64_t modpow(uint64_t base, uint64_t exp,uint64_t prime);



    uint64_t decompress(const std::string& file);
}



#endif //RRPAIR_UTILS_HPP
