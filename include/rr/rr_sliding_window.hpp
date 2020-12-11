//
// Created by via56 on 30-11-20.
//

#ifndef RRPAIR_RR_SLIDING_WINDOW_HPP
#define RRPAIR_RR_SLIDING_WINDOW_HPP


#include <cstdint>
#include "../fingerprints/fingerprints.hpp"
#include "rr_utils.hpp"

namespace rr{


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

    typedef krb_fingerprint_windows<uchar,uint64_t,fingerprints::kr_hash_uc32> w_kr_uc32;
    typedef krb_fingerprint_windows<uchar,uint64_t,fingerprints::kr_hash_uc64> w_kr_uc64;


    template <>
    void destroy<w_kr_uc32>( w_kr_uc32& );
    template <>
    void init<w_kr_uc32>( const uint32_t & size,w_kr_uc32& );
    template <>
    uint64_t feed<unsigned char,w_kr_uc32>( const unsigned char&, w_kr_uc32& );
    template <>
    void reset<w_kr_uc32>(  w_kr_uc32& );
    template <>
    uint64_t size_window<w_kr_uc32>( w_kr_uc32& );


    template <>
    void destroy<w_kr_uc64>( w_kr_uc64& );
    template <>
    void init<w_kr_uc64>( const uint32_t & size,w_kr_uc64& );
    template <>
    uint64_t  feed<unsigned char,w_kr_uc64>( const unsigned char&, w_kr_uc64& );
    template <>
    void reset<w_kr_uc64>(  w_kr_uc64& );
    template <>
    uint64_t size_window<w_kr_uc64>( w_kr_uc64& );





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

//        KR_window(int w, int bxs): wsize(w), bytexsymb(bxs) {
//            wbsize = wsize*bytexsymb;    // size of window in bytes
//            asize = 256;                 // alphabet size for bytes
//            asize_pot = modpow(asize,wbsize-1); // power used to update hash when oldest char exit
//            // alloc and clear window
//            window = new uint8_t[wbsize];
//            reset();
//        }
//
//        // power modulo prime
//        uint64_t modpow(uint64_t base, uint64_t exp)
//        {
//            assert(exp>0);
//            if(exp==1) return base;
//            if(exp%2==0)
//                return modpow((base*base)%prime,exp/2);
//            else
//                return (base*modpow((base*base)%prime,exp/2)) % prime;
//        }
//
//        // init window, hash, and tot_symb
//        void reset() {
//            for(int i=0;i<wbsize;i++) window[i]=0;
//            // init hash value and related values
//            hash=tot_symb=0;
//        }
//
//        // add a symbol consisting of bytexsymbol uint8's to the window
//        // update and return the hash for the resulting window
//        uint64_t addsymbol(uint8_t *s) {
//            // compute destination of symbol's bytes inside window[]
//            int k = (tot_symb++ % wsize)*bytexsymb;
//            assert(k+bytexsymb-1<wbsize); // make sure we are inside window[]
//            for(int i=0;i<bytexsymb;i++) {
//                // complex expression to avoid negative numbers
//                hash += (prime - (window[k]*asize_pot) % prime); // remove window[k] contribution
//                hash = (asize*hash + s[i]) % prime;      //  add char i
//                window[k++]=s[i];
//            }
//            return hash;
//        }
//
//        ~KR_window() {
//            delete[] window;
//        }

    };



    template <>
    void destroy<KR_window>( KR_window& );
    template <>
    void init<KR_window>( const uint32_t & size,KR_window& );
    template <>
    uint64_t  feed<unsigned char,KR_window>( const unsigned char&, KR_window& );
    template <>
    void reset<KR_window>(  KR_window& );
    template <>
    uint64_t size_window<KR_window>( KR_window& );



}




#endif //RRPAIR_RR_SLIDING_WINDOW_HPP
