//
// Created by via56 on 29-11-20.
//

#ifndef RRPAIR_RR_UTILS_HPP
#define RRPAIR_RR_UTILS_HPP


#include <cstdint>

namespace rr{




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

}



#endif //RRPAIR_RR_UTILS_HPP
