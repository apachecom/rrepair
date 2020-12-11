//
// Created by via56 on 29-11-20.
//

#ifndef RRPAIR_FINGERPRINTS_HPP
#define RRPAIR_FINGERPRINTS_HPP

#include <cstdint>
#include <string>
#include <cassert>
#include <vector>

namespace fingerprints {

    typedef unsigned __int128 uint128_t;

    template<
            typename c_type,
            typename hash_type
    >
    class kr_hash {
    public:

        kr_hash(const uint64_t &_p =  1999999973
                 ) {
            prime = _p;
        }

        uint64_t prime;
        hash_type hash(const std::basic_string<c_type> &);
    };

    template<>
    class kr_hash<unsigned char, uint64_t> {
    public:

        kr_hash(const uint64_t &_p =  1999999973
        ) {
            prime = _p;
        }
        uint64_t prime;
        uint64_t hash(const std::basic_string<unsigned char> &str);
    };

    template<>
    class kr_hash<char, uint64_t> {
    public:

        kr_hash(const uint64_t &_p =  1999999973
        ) {
            prime = _p;
        }
        uint64_t prime;
        uint64_t hash(const std::basic_string<char> &str);
    };



    template<>
    class kr_hash<uint32_t , uint64_t> {
    public:
        kr_hash(const uint64_t &_p =  3355443229//1999999973
        ) {
            prime = _p;
        }
        uint64_t prime;
        uint64_t hash(const std::basic_string<uint32_t> &str);
    };



    class MzzHash{

    // compute 64-bit KR hash of a string
    // to avoid overflows in 64 bit aritmethic the prime is taken < 2**55
        public:

            uint64_t operator()(const std::basic_string<unsigned char> &s) const{
                return this->hash(s);
            }
            uint64_t operator()(const std::basic_string<char> &s) const{
                return this->hash(s);
            }

            uint64_t hash(const std::basic_string<unsigned char> &s)const {
                uint64_t hash = 0;
                //const uint64_t prime = 3355443229;     // next prime(2**31+2**30+2**27)
                const uint64_t prime = 27162335252586509; // next prime (2**54 + 2**53 + 2**47 + 2**13)
                for(size_t k=0;k<s.size();k++) {
                    int c = (unsigned char) s[k];
                    assert(c>=0 && c< 256);
                    hash = (256*hash + c) % prime;    //  add char k
                }
                return hash;

            }
            uint64_t hash(const std::basic_string<char> &s)const {
                uint64_t hash = 0;
                //const uint64_t prime = 3355443229;     // next prime(2**31+2**30+2**27)
                const uint64_t prime = 27162335252586509; // next prime (2**54 + 2**53 + 2**47 + 2**13)
                for(size_t k=0;k<s.size();k++) {
                    int c = (unsigned char)s[k];
                    assert(c>=0 && c< 256);
                    hash = (256*hash + c) % prime;    //  add char k
                }
                return hash;

            }

            uint64_t hash(const std::vector<uint8_t> &s)const {
                uint64_t hash = 0;
                //const uint64_t prime = 3355443229;     // next prime(2**31+2**30+2**27)
                const uint64_t prime = 27162335252586509; // next prime (2**54 + 2**53 + 2**47 + 2**13)
                for(size_t k=0;k<s.size();k++) {
                    int c = (unsigned char)s[k];
                    assert(c>=0 && c< 256);
                    hash = (256*hash + c) % prime;    //  add char k
                }
                return hash;

            }
    };



    typedef fingerprints::kr_hash<unsigned char,uint64_t> kr_hash_uc64;
    typedef fingerprints::kr_hash<unsigned char,uint32_t> kr_hash_uc32;







}


#endif //RRPAIR_FINGERPRINTS_HPP
