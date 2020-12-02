//
// Created by via56 on 29-11-20.
//
#include "../include/rr_sliding_window.hpp"



/**
 * Implemetation
 *
 * */


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

    window.asize = 1ULL << 32;
    window.asize_pot = 1;
    window.fhash.prime = 1999999973;
    window.wsize = size;
    for (uint64_t i = 1; i < size; i++)
        window.asize_pot = (window.asize_pot * window.asize) % window.fhash.prime; // ugly linear-time power algorithm
    // alloc and clear window
    window.window = new unsigned char[size];
    for (uint64_t i = 0; i < size; i++) window.window[i] = 0;
    // init hash value and related values
    window.hash = window.tot_char = 0;
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

