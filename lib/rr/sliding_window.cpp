//
// Created by via56 on 29-11-20.
//
#include "../../include/rr/sliding_window.hpp"
#include "../../DummyPartition.h"


using namespace rr;
// init window, hash, and tot_symb
template <>
void rr::reset<rr::KR_window>(KR_window& window) {
    for(int i=0;i<window.wbsize;i++) window.window[i]=0;
    // init hash value and related values
    window.hash=window.tot_symb=0;
}
template <>
void rr::init<rr::KR_window>( const uint32_t & size,KR_window& window){

    window.wsize = size;
    window.bytexsymb = 1;
    window.wbsize = window.wsize*window.bytexsymb;    // size of window in bytes
    window.asize = 256;                 // alphabet size for bytes
    window.asize_pot = modpow(window.asize,window.wbsize-1, window.prime); // power used to update hash when oldest char exit
    // alloc and clear window
    window.window = new uint8_t[window.wbsize];
    reset(window);

}

//template <>
//uint64_t rr::feed<uint8_t,rr::KR_window>(const uint8_t& s, KR_window& window ){
//
//    // compute destination of symbol's bytes inside window[]
//    int k = (window.tot_symb++ % window.wsize)*window.bytexsymb;
//    assert(k+window.bytexsymb-1<window.wbsize); // make sure we are inside window[]
//    for(int i=0;i<window.bytexsymb;i++) {
//        // complex expression to avoid negative numbers
//        window.hash += (window.prime - (window.window[k]*window.asize_pot) % window.prime); // remove window[k] contribution
//        window.hash = (window.asize*window.hash + s) % window.prime;      //  add char i
//        window.window[k++] = s;
//    }
//    return window.hash;
//}

template <>
uint64_t rr::feed<uint8_t ,rr::KR_window>(const uint8_t * s, KR_window& window ){

    // compute destination of symbol's bytes inside window[]
    int k = ((++window.tot_symb) % window.wsize)*window.bytexsymb;
    assert(k+window.bytexsymb-1<window.wbsize); // make sure we are inside window[]
    for(int i=0;i<window.bytexsymb;i++) {
    // complex expression to avoid negative numbers
        window.hash += (window.prime - (window.window[k]*window.asize_pot) % window.prime); // remove window[k] contribution
        window.hash = (window.asize*window.hash + s[i]) % window.prime;      //  add char i
        window.window[k++] = s[i];
    }
    return window.hash;
}

template <>
uint64_t rr::size_window<rr::KR_window>(KR_window& window){
    return (uint64_t)window.wsize;
}
template <>
void rr::destroy<rr::KR_window>( rr::KR_window& w){
    delete[] w.window;
}
