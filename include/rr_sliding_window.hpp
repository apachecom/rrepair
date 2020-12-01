//
// Created by via56 on 30-11-20.
//

#ifndef RRPAIR_RR_SLIDING_WINDOW_HPP
#define RRPAIR_RR_SLIDING_WINDOW_HPP


#include <cstdint>

namespace parse{


    // recibe size of the windows
    template <typename c_type, typename hash_type, typename W>
    void init(const uint32_t & , W&);

    template <typename c_type, typename hash_type, typename W>
    uint64_t size_window(W&);

    template <typename c_type, typename hash_type, typename W>
    uint64_t feed(const c_type&, W& );

    template <typename c_type, typename hash_type, typename W>
    void reset( W& );

    template <typename c_type, typename hash_type, typename W>
    void destroy( W& );

    template <
                typename c_type,
                typename hash_type,
                typename hash_f,
                uint64_t prime = 1999999973

//            const hash_type  prime = 27162335252586509;
                        >
    struct krb_fingerprint_windows{

        int wsize;
        c_type *window;
        int asize;
        hash_type hash;
        hash_type tot_char;
        hash_type asize_pot;   // asize^(wsize-1) mod prime
        hash_f fhash;
    };

}




#endif //RRPAIR_RR_SLIDING_WINDOW_HPP
