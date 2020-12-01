//
// Created by via56 on 29-11-20.
//
#include "../include/rr_sliding_window.hpp"
#include "../include/rr_fingerprints.hpp"

typedef unsigned char uchar;
typedef fingerprints::kr_hash<unsigned char,uint64_t> kr_hash_uc64;
typedef fingerprints::kr_hash<unsigned char,uint32_t> kr_hash_uc32;

typedef parse::krb_fingerprint_windows<uchar,uint64_t,kr_hash_uc32> w_kr_uc32;

template <>
void parse::destroy<unsigned char,uint64_t,w_kr_uc32>( w_kr_uc32& );

template <>
void parse::init<unsigned char,uint64_t,w_kr_uc32>( const uint32_t & size,w_kr_uc32& );

template <>
uint64_t  parse::feed<unsigned char,uint64_t,w_kr_uc32>( const unsigned char&, w_kr_uc32& );

template <>
void parse::reset<unsigned char,uint64_t,w_kr_uc32>(  w_kr_uc32& );

template <>
uint64_t parse::size_window<unsigned char,uint64_t,w_kr_uc32>( w_kr_uc32& );


/**
 * Implemetation
 *
 * */


template <>
void parse::destroy<unsigned char,uint64_t,w_kr_uc32>( w_kr_uc32& window){
    delete[] window.window;
}


template <>
void parse::init<unsigned char,uint64_t,w_kr_uc32>( const uint32_t & size, w_kr_uc32& window){

    window.asize = 256;
    window.asize_pot = 1;
    window.fhash.prime = 1999999973;
    window.wsize = size;
    for (int i = 1; i < size; i++)
        window.asize_pot = (window.asize_pot * window.asize) % window.fhash.prime; // ugly linear-time power algorithm
    // alloc and clear window
    window.window = new unsigned char[size];
    for (int i = 0; i < size; i++) window.window[i] = 0;
    // init hash value and related values
    window.hash = window.tot_char = 0;
}

template <>
uint64_t parse::feed<unsigned char,uint64_t,w_kr_uc32>( const unsigned char& c, w_kr_uc32& window){

    int k = window.tot_char++ % window.wsize;
    // complex expression to avoid negative numbers
    window.hash += (window.fhash.prime - (window.window[k] * window.asize_pot) % window.fhash.prime); // remove window[k] contribution
    window.hash = (window.asize * window.hash + c) % window.fhash.prime;      //  add char i
    window.window[k] = c;
    // cerr << get_window() << " ~~ " << window << " --> " << hash << endl;
    return window.hash;
}

template <>
void parse::reset<unsigned char,uint64_t,w_kr_uc32>(  w_kr_uc32& windows ){
    for (uint32_t i = 0; i < windows.wsize; i++)
        windows.window[i] = 0;
    // init hash value and related values
    windows.hash = windows.tot_char = 0;
}



