//
// Created by via56 on 29-11-20.
//


#include "../../include/rr/utils.hpp"
#include <cassert>


// power modulo prime
uint64_t rr::modpow(uint64_t base, uint64_t exp,uint64_t prime)
{
    assert(exp>0);
    if(exp==1) return base;
    if(exp%2==0)
        return modpow((base*base)%prime,exp/2,prime);
    else
        return (base*modpow((base*base)%prime,exp/2,prime)) % prime;
}
