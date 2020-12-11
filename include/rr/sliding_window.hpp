//
// Created by via56 on 30-11-20.
//

#ifndef RRPAIR_SLIDING_WINDOW_HPP
#define RRPAIR_SLIDING_WINDOW_HPP


#include <cstdint>
#include "../fingerprints/fingerprints.hpp"
#include "utils.hpp"

namespace rr{


    // recibe size of the windows
    template < typename W>
    void init(const uint32_t & , W&);

    template <typename W>
    uint64_t size_window(W&);

//    template <typename c_type, typename W>
//    uint64_t feed(const c_type&, W& );

    template <typename c_type, typename W>
    uint64_t feed(const c_type*, W& );

    template <typename W>
    void reset( W& );

    template < typename W>
    void destroy( W& );
//
//    template <
//                typename c_type,
//                typename hash_type,
//                typename hash_f,
//                uint64_t prime = 1999999973
//
////            const hash_type  prime = 27162335252586509;
//                        >
//    struct krb_fingerprint_windows{
//
//        uint64_t wsize;
//        c_type *window;
//        uint64_t  asize;
//        hash_type hash;
//        hash_type tot_char;
//        hash_type asize_pot;   // asize^(wsize-1) mod prime
//        hash_f fhash;
//    };

    typedef unsigned char uchar;

//    typedef krb_fingerprint_windows<uchar,uint64_t,fingerprints::kr_hash_uc32> w_kr_uc32;
//    typedef krb_fingerprint_windows<uchar,uint64_t,fingerprints::kr_hash_uc64> w_kr_uc64;

    struct KR_window {
        int wsize;            // number of symbols in window
        int bytexsymb;        // number of bytes per symbol
        int wbsize;           // size of window in bytes
        uint8_t *window;
        int asize;            // alphabet size
        const uint64_t prime = 1999999973; // slightly less that 2^31
        uint64_t hash;        // hash of the symbols currently in window
        uint64_t tot_symb;    // char added so far, equals symbols*bxs
        uint64_t asize_pot;   // asize^(wsize-1) mod prime
        fingerprints::MzzHash fhash;
    };


    template <>
    void destroy<KR_window>( KR_window& );
    template <>
    void init<KR_window>( const uint32_t & size,KR_window& );
//    template <>
//    uint64_t  feed<uint8_t ,KR_window>( const uint8_t &, KR_window& );
    template <>
    uint64_t  feed<uint8_t ,KR_window>( const uint8_t *, KR_window& );
//    template <>
//    uint64_t  feed<unsigned char ,KR_window>( const unsigned char*, KR_window& );
//    template <>
//    uint64_t  feed< char ,KR_window>( const char*, KR_window& );
    template <>
    void reset<KR_window>(  KR_window& );
    template <>
    uint64_t size_window<KR_window>( KR_window& );



}




#endif //RRPAIR_SLIDING_WINDOW_HPP
