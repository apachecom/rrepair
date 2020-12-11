//
// Created by via56 on 29-11-20.
//

#include "../../include/fingerprints/fingerprints.hpp"
using namespace fingerprints;

uint64_t fingerprints::kr_hash<unsigned char, uint64_t>::hash(const std::basic_string<unsigned char> &str) {
    uint64_t hash = 0;
    for (size_t k = 0; k < str.size(); k++) {
        int c = str[k];
        assert(c >= 0 && c < 256);
        hash = (256 * hash + c) % prime;    //  add char k
    }
    return hash;
}

uint64_t fingerprints::kr_hash<char, uint64_t>::hash(const std::basic_string<char> &str) {
    uint64_t hash = 0;
    for (size_t k = 0; k < str.size(); k++) {
        int c = (unsigned char) str[k];
//        assert(c >= 0 && c < 256);
        hash = (256 * hash + c) % prime;    //  add char k
    }
    return hash;
}

uint64_t fingerprints::kr_hash<uint32_t, uint64_t>::hash(const std::basic_string<uint32_t> &str) {
    uint64_t hash = 0;
    uint64_t shift = (1UL << 32);
    for (size_t k = 0; k < str.size(); k++) {
        uint32_t c = str[k];
//        assert(c >= 0 && c < 256);
        hash = ( shift * hash + c) % prime;    //  add char k
    }
    return hash;
};