//
// Created by via56 on 30-11-20.
//

#ifndef RRPAIR_RR_SLIDING_WINDOW_HPP
#define RRPAIR_RR_SLIDING_WINDOW_HPP


#include <cstdint>
#include "../include/rr_fingerprints.hpp"


namespace parse{


    // recibe size of the windows
    template < typename W>
    void init(const uint32_t & , W&);

    template <typename W>
    uint64_t size_window(W&);

    template <typename c_type, typename W>
    uint64_t feed(const c_type&, W& );

    template <typename W>
    void reset( W& );

    template < typename W>
    void destroy( W& );

    template <
                typename c_type,
                typename hash_type,
                typename hash_f,
                uint64_t prime = 1999999973

//            const hash_type  prime = 27162335252586509;
                        >
    struct krb_fingerprint_windows{

        uint64_t wsize;
        c_type *window;
        uint64_t  asize;
        hash_type hash;
        hash_type tot_char;
        hash_type asize_pot;   // asize^(wsize-1) mod prime
        hash_f fhash;
    };

    typedef unsigned char uchar;
    typedef fingerprints::kr_hash<unsigned char,uint64_t> kr_hash_uc64;
    typedef fingerprints::kr_hash<unsigned char,uint32_t> kr_hash_uc32;

    typedef parse::krb_fingerprint_windows<uchar,uint64_t,kr_hash_uc32> w_kr_uc32;
    typedef parse::krb_fingerprint_windows<uchar,uint64_t,kr_hash_uc64> w_kr_uc64;


    template <>
    void parse::destroy<w_kr_uc32>( w_kr_uc32& );

    template <>
    void parse::init<w_kr_uc32>( const uint32_t & size,w_kr_uc32& );

    template <>
    uint64_t  parse::feed<unsigned char,w_kr_uc32>( const unsigned char&, w_kr_uc32& );

    template <>
    void parse::reset<w_kr_uc32>(  w_kr_uc32& );

    template <>
    uint64_t parse::size_window<w_kr_uc32>( w_kr_uc32& );



    template <>
    void parse::destroy<w_kr_uc64>( w_kr_uc64& );

    template <>
    void parse::init<w_kr_uc64>( const uint32_t & size,w_kr_uc64& );

    template <>
    uint64_t  parse::feed<unsigned char,w_kr_uc64>( const unsigned char&, w_kr_uc64& );

    template <>
    void parse::reset<w_kr_uc64>(  w_kr_uc64& );

    template <>
    uint64_t parse::size_window<w_kr_uc64>( w_kr_uc64& );


}




#endif //RRPAIR_RR_SLIDING_WINDOW_HPP
