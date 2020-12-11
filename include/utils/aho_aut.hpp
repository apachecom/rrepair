//
// Created by via56 on 06-12-20.
//

#ifndef RRPAIR_AHO_AUT_HPP
#define RRPAIR_AHO_AUT_HPP


#include "../trees/trie/Trie.h"
#include <cmath>
#include <set>

namespace utils {



    //recive pattern
    template <typename A>
    void match(const std::string &, A& );
    //recive text
    template <typename A>
    void build(const std::vector<std::string> &, A& );

    //
    //Efficient String Matching:An Aid to Bibliographic Search
    //Alfred V. Aho and Margaret J. Corasick
    template <
            typename G,
            typename F,
            typename O
    >
    class AhoAutPM {
    public:
        AhoAutPM() = default;
        ~AhoAutPM() = default;
        G _goTo; // goto function (state,sym) -> state
        F _fail; // failure function (state) -> state
        O _output; // output function
    };

    struct state{
        uint32_t id;
        std::unordered_map<char,uint32_t> _map;
    };

    typedef std::set<uint32_t> _MapOutPut;
    typedef std::unordered_map<uint32_t,state> _MapGoto;
    typedef std::unordered_map<uint32_t,uint32_t> _MapFailure;

    template <>
    class AhoAutPM<_MapGoto,_MapFailure,_MapOutPut>{

        public:
            _MapGoto _goTo; // goto function (state,sym) -> state
            _MapFailure _fail; // failure function (state) -> state
            _MapOutPut _output; // output function
    };


    typedef AhoAutPM<_MapGoto,_MapFailure,_MapOutPut> AM;
    template <>
    void match<AM>(const std::string & text, AM& aho_aut);
    template <>
    void build<AM>(const std::vector<std::string> & dicc, AM& aut );
}



#endif //RRPAIR_AHO_AUT_HPP
