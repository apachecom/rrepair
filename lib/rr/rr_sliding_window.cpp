//
// Created by via56 on 29-11-20.
//
#include "../../include/rr/rr_sliding_window.hpp"
#include "../../DummyPartition.h"


using namespace parse;



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


/**
 * Implemetation
 *
 * */
// power modulo prime
uint64_t modpow(uint64_t base, uint64_t exp,uint64_t prime)
{
    assert(exp>0);
    if(exp==1) return base;
    if(exp%2==0)
        return modpow((base*base)%prime,exp/2,prime);
    else
        return (base*modpow((base*base)%prime,exp/2,prime)) % prime;
}



template <>
void parse::destroy<parse::w_kr_uc32>( parse::w_kr_uc32& window){
    delete[] window.window;
}


template <>
void parse::init<parse::w_kr_uc32>( const uint32_t & size, parse::w_kr_uc32& window){

    window.asize = 1ULL << 32;
    window.asize_pot = 1;
    window.fhash.prime = 1999999973;
    window.wsize = size;
    for (uint32_t i = 1; i < size; i++)
        window.asize_pot = (window.asize_pot * window.asize) % window.fhash.prime; // ugly linear-time power algorithm
    // alloc and clear window
    window.window = new unsigned char[size];
    for (uint32_t i = 0; i < size; i++) window.window[i] = 0;
    // init hash value and related values
    window.hash = window.tot_char = 0;
}

template <>
uint64_t parse::feed<unsigned char,parse::w_kr_uc32>( const unsigned char& c, parse::w_kr_uc32& window){

    uint32_t k = window.tot_char++ % window.wsize;
    // complex expression to avoid negative numbers
    window.hash += (window.fhash.prime - (window.window[k] * window.asize_pot) % window.fhash.prime); // remove window[k] contribution
    window.hash = (window.asize * window.hash + c) % window.fhash.prime;      //  add char i
    window.window[k] = c;
    // cerr << get_window() << " ~~ " << window << " --> " << hash << endl;
    return window.hash;
}

template <>
void parse::reset<parse::w_kr_uc32>(  parse::w_kr_uc32& windows ){
    for (uint32_t i = 0; i < windows.wsize; i++)
        windows.window[i] = 0;
    // init hash value and related values
    windows.hash = windows.tot_char = 0;
}


template <>
uint64_t parse::size_window<parse::w_kr_uc32>( parse::w_kr_uc32& windows){
    return windows.wsize;
}




template <>
void parse::destroy<parse::w_kr_uc64>( parse::w_kr_uc64& window){
    delete[] window.window;
}


template <>
void parse::init<parse::w_kr_uc64>( const uint32_t & size, parse::w_kr_uc64& window){
    window.asize = 256;
    window.wsize = size;
    window.fhash.prime = 1999999973;
    window.asize_pot = modpow(window.asize,window.wsize-1,window.fhash.prime);
    window.window = new uint8_t[size];
    reset(window);
}

template <>
uint64_t parse::feed<unsigned char,parse::w_kr_uc64>( const unsigned char& c, parse::w_kr_uc64& window){

    uint64_t k = window.tot_char++ % window.wsize;
    // complex expression to avoid negative numbers
    window.hash += (window.fhash.prime - (window.window[k] * window.asize_pot) % window.fhash.prime); // remove window[k] contribution
    window.hash = (window.asize * window.hash + c) % window.fhash.prime;      //  add char i
    window.window[k] = c;
    // cerr << get_window() << " ~~ " << window << " --> " << hash << endl;
    return window.hash;
}

template <>
void parse::reset<parse::w_kr_uc64>(  parse::w_kr_uc64& windows ){
    for (uint64_t i = 0; i < windows.wsize; i++)
        windows.window[i] = 0;
    // init hash value and related values
    windows.hash = windows.tot_char = 0;
}


template <>
uint64_t parse::size_window<parse::w_kr_uc64>( parse::w_kr_uc64& windows){
    return windows.wsize;
}




// init window, hash, and tot_symb

template <>
void parse::reset<parse::KR_window>(KR_window& window) {
    for(int i=0;i<window.wbsize;i++) window.window[i]=0;
    // init hash value and related values
    window.hash=window.tot_symb=0;
}


template <>
void parse::init<parse::KR_window>( const uint32_t & size,KR_window& window){

    window.wsize = size;
    window.bytexsymb = 1;
    window.wbsize = window.wsize*window.bytexsymb;    // size of window in bytes
    window.asize = 256;                 // alphabet size for bytes
    window.asize_pot = modpow(window.asize,window.wbsize-1, window.prime); // power used to update hash when oldest char exit
    // alloc and clear window
    window.window = new uint8_t[window.wbsize];
    reset(window);

}


template <>
uint64_t parse::feed<unsigned char,parse::KR_window>(const unsigned char& s, KR_window& window ){

    // compute destination of symbol's bytes inside window[]
    int k = (window.tot_symb++ % window.wsize)*window.bytexsymb;
    assert(k+window.bytexsymb-1<window.wbsize); // make sure we are inside window[]
//    for(int i=0;i<window.bytexsymb;i++) {
        // complex expression to avoid negative numbers
        window.hash += (window.prime - (window.window[k]*window.asize_pot) % window.prime); // remove window[k] contribution
        window.hash = (window.asize*window.hash + s) % window.prime;      //  add char i
        window.window[k++] = s;
//    }
    return window.hash;
}


template <>
uint64_t parse::size_window<parse::KR_window>(KR_window& window){ return (uint64_t)window.wsize;}
template <>
void parse::destroy<parse::KR_window>( parse::KR_window& w){
    delete[] w.window;
}
