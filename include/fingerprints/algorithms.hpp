//
// Created by via56 on 05-12-20.
//

#ifndef RRPAIR_ALGORITHMS_HPP
#define RRPAIR_ALGORITHMS_HPP




#include <cmath>
#include <set>
#include <cstdint>
#include <string>
#include <map>

namespace fingerprint{


        template <typename T = uint64_t>
        struct d_elemt{
            T k {0};
            T L {0};
            T R {0};
            d_elemt() = default;
            d_elemt(T& _k, T& _L, T& _R):k(_k),L(_L),R(_R){}
        };

        template <uint32_t t = 32,typename F, typename AC>
        bool verify_tgood_fingerprint(const char * T, const uint64_t& len) {

            AC aho_aut;
            std::map<uint64_t,d_elemt<>> D;
            uint64_t _log_n_t = log2(len/t);
            // outermost loop
            for (uint64_t l = 0; l < _log_n_t; ++l) {

                uint64_t p_2_l =  len/t;
                uint64_t m = len/t - p_2_l;
                for (uint64_t j = 0; j < m; ++j) {

                    uint64_t a = ( j + p_2_l/2 )*t;
                    uint64_t b = ( j + p_2_l )*t;
                    uint64_t f_j = F(T,j*t, b - 1);

                    d_elemt<> _e( F(T,j*t, a - 1), F(T,  a, b - 1), j);

                    if(D.find(f_j) == D.end()){
                        D[f_j] = _e;
                        if(l == 0){
                            //Insert T [jτ . . . (j + 1)τ − 1] into AC dictionary
                            std::string str; str.resize(t);
                            std::copy(T + j*t,T+(j+1)*t,str.begin());
                            aho_aut.push(str);// feed into AC
                        }

                    }

                }

                if(l == 0){
                    std::string str; str.resize(t);
                    std::copy(T,T+t,str.begin());
                    aho_aut.push_back(str); // feed into AC
                }
                //

                uint32_t p_l = (1 << l);
                m = len - t*p_l + 1;

                for (uint32_t i = 0; i < m; ++i) {

                    uint64_t f_i = F(T,i, i + t*p_l - 1); // fj F(T[j*t ... ( j+2^l )*t - 1 ])
                    uint64_t L_i = F(T,i, i + t*p_l/2 - 1); // fj F(T[j*t ... ( j+2^l )*t - 1 ])
                    uint64_t R_i = F(T,(i+p_l/2)*t,i+t*p_l-1); // fj F(T[j*t ... ( j+2^l )*t - 1 ])

                    if(l == 0){
                        //feed T[i ... i + t -1 ] in AC
                        std::string str; str.resize(t);
                        std::copy(T+i,T+i+t,str.begin());
                        aho_aut.feed(str);
                    }

                    auto it_d = D.find(f_i);
                    if(it_d != D.end()){
                        if(l > 0){
                            if(it_d->second.L != L_i || it_d->second.R != R_i)
                                return false;
                        }
                        else{
                            // compare T[i...i + τ − 1] to T [kτ ... (k + 1)τ − 1]
                            std::string str; str.resize(t);
                            std::copy(T+i,T+i+t,str.begin());
//                        if( aho_aut.check())
                        }
                    }
                }



            }

        }
    }







#endif //RRPAIR_ALGORITHMS_HPP
