//
// Created by ale on 01-07-20.
//

#ifndef RRPAIR_HASHFUNCTION_H
#define RRPAIR_HASHFUNCTION_H


#include <cstdint>
#include <string>
#include <cassert>

namespace big_repair {



    template<
            typename value_hash_type,
            typename key_hash_type
    >
    class KRPHashFunction {

    public:
        KRPHashFunction() = default;

        ~KRPHashFunction() = default;

        virtual value_hash_type apply(const key_hash_type &) const = 0;
    };

    template <>
    class KRPHashFunction <uint64_t ,std::string> {

        public:

            KRPHashFunction() = default;
            ~KRPHashFunction() = default;
            uint64_t apply(const std::string & s) const {

                uint64_t hash = 0;
                //const uint64_t prime = 3355443229;     // next prime(2**31+2**30+2**27)
                const uint64_t prime = 27162335252586509; // next prime (2**54 + 2**53 + 2**47 + 2**13)

                for (size_t k = 0; k < s.size(); k++) {
                    int c = (unsigned char) s[k];
                    assert(c >= 0 && c < 256);

                    hash = (256 * hash + c) % prime;    //  add char k
                }
                return hash;

            }

            uint64_t apply(const char *s, uint len) const {

                uint64_t hash = 0;
                //const uint64_t prime = 3355443229;     // next prime(2**31+2**30+2**27)
                const uint64_t prime = 27162335252586509; // next prime (2**54 + 2**53 + 2**47 + 2**13)

                for (size_t k = 0; k < len; k++) {

                    int c = (unsigned int) s[k];
                    //  std::cout<<c<<" ";
                    assert(c >= 0 && c < 256);
                    hash = (256 * hash + c) % prime;    //  add char k
                }
                //std::cout<<std::endl;

                return hash;
            }
    };

}
#endif //RRPAIR_HASHFUNCTION_H
