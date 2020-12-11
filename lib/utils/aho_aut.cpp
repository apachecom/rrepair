//
// Created by via56 on 06-12-20.
//


#include "../../include/utils/aho_aut.hpp"

#include <set>

using namespace utils;
typedef AhoAutPM<_MapGoto,_MapFailure,_MapOutPut> AM;

template <>
void utils::match<AM>(const std::string & text, AM& aho_aut){
//todo implemetar el metodo


    uint32_t state_id = 0;
    uint64_t  n = text.size();
    for (uint32_t i = 0; i < n; ++i) {
        auto it_state = aho_aut._goTo[state_id];
        auto it_state_map = it_state._map.find(text[i]);

        while(it_state_map == it_state._map.end()){

            state_id = aho_aut._fail[state_id];
            it_state = aho_aut._goTo[state_id];
            it_state_map = it_state._map.find(text[i]);
        }
        auto state = aho_aut._goTo[it_state_map->second];


    }

}


// todo test the method
template <>
void utils::build<AM>(const std::vector<std::string> & dicc, AM& aut ){

    // insert start state
    state init;
    uint32_t id_inc = 0;
    init.id = id_inc++;
    // iterate on dicc and insert as a trie
    // map to store the state id by deep state (usefull to compute the failure function)
    // map deep -> (id,(parent,sym))
    std::map<uint32_t,std::vector<std::pair<uint32_t,std::pair<uint32_t,char>>>> deep;
    deep[0].emplace_back(init.id,std::make_pair(0,0));
    for (const auto &item : dicc) {
        uint32_t current = init.id;
        for( uint32_t ii = 0 ; ii < item.length() ; ii++){
            auto it = aut._goTo[current]._map.find(item[ii]);
            if(it == aut._goTo[current]._map.end()){
                //case new edge
                // create new state
                state new_st;new_st.id = id_inc++;
                // assign edge
                aut._goTo[current]._map[item[ii]] = new_st.id;
                // assign state
                aut._goTo[new_st.id] = new_st;
                current = new_st.id;
                // store the deep of the state
                deep[ii+1].emplace_back(new_st.id,std::make_pair(current,item[ii]));

            }else{
                // already exist the edge
                current = it->second;
            }
        }
    }


    //  build failure function...
    for (const auto &d : deep) {
        if(d.first == 0)
            // for deep 0 base case
            aut._fail[0] = 0;
        else {
            // for deep greater than 1
            uint32_t len = d.second.size();
            // check all state in current deep
            for (uint32_t i = 0; i < len ; ++i) {
                auto p = d.second[i];

                char c = p.second.second; // char
                uint32_t f_state = p.second.first; // previous state

                // go back until reach the root or find a node with a edge (c,st) such tha c is the incoming character of the state
                while(aut._goTo[aut._fail[f_state]]._map.find(c) == aut._goTo[aut._fail[f_state]]._map.end() && f_state != 0)
                    f_state = aut._fail[f_state];
                // if we found the edge
                if(aut._goTo[aut._fail[f_state]]._map.find(c) == aut._goTo[aut._fail[f_state]]._map.end())
                    aut._fail[p.first] = aut._goTo[aut._fail[f_state]]._map[c];
                else
                    aut._fail[p.first] = 0;
            }
        }
    }
}
