//
// Created by ale on 01-07-20.
//

#ifndef RRPAIR_SLIDINGWINDOW_H
#define RRPAIR_SLIDINGWINDOW_H


#include <cstdint>
#include "RepairUtils.h"

namespace big_repair{


    template < typename HashFunction, typename hash_type = uint64_t>
    class SlidingWindow{
        public:
            SlidingWindow() = default;
            ~SlidingWindow() = default;
            virtual const uint_t sizeWindow() = 0;
            virtual const hash_type hashAddCharToWindow(const int& c ) = 0;
            virtual void reset() = 0;
    };


    template <typename hash_type = uint64_t>
    class KRPSlindingWindow  {

        protected:
            int wsize;
            int *window;
            int asize;
//            const hash_type  prime = 27162335252586509;
            hash_type prime;
            hash_type hash;
            hash_type tot_char;
            hash_type asize_pot;   // asize^(wsize-1) mod prime



        public:

            KRPSlindingWindow() {

                wsize = 0;
                window = nullptr;
                asize = 0;
                prime = 27162335252586509;
//                prime = 1999999973;
                hash = 0;
                tot_char = 0;
                asize_pot = 0;
            }

            KRPSlindingWindow(const int& ws ) : wsize(ws){

                asize = 256;
                asize_pot = 1;
//                prime = 1999999973;
                prime = 27162335252586509;
                for (int i = 1; i < wsize; i++)
                    asize_pot = (asize_pot * asize) % prime; // ugly linear-time power algorithm
                // alloc and clear window
                window = new int[wsize];

                for (int i = 0; i < wsize; i++) window[i] = 0;
                // init hash value and related values
                hash = tot_char = 0;

            }

            ~KRPSlindingWindow() {    delete[] window; }

            virtual uint_t sizeWindow() const { return wsize;}

            virtual hash_type hashAddCharToWindow(const int& c ) {

                int k = tot_char++ % wsize;
                // complex expression to avoid negative numbers
                hash += (prime - (window[k] * asize_pot) % prime); // remove window[k] contribution
                hash = (asize * hash + c) % prime;      //  add char i
                window[k] = c;
                // cerr << get_window() << " ~~ " << window << " --> " << hash << endl;
                return hash;


            }

            virtual void reset() {
                for (int i = 0; i < wsize; i++) window[i] = 0;
                // init hash value and related values
                hash = tot_char = 0;

            }




    };








}

#endif //RRPAIR_SLIDINGWINDOW_H
