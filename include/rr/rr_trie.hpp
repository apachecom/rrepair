//
// Created by via56 on 30-11-20.
//

#ifndef RRPAIR_RR_TRIE_HPP
#define RRPAIR_RR_TRIE_HPP
#include <vector>
#include <unordered_map>

namespace rr{

    template <typename K = uint64_t ,typename C = unsigned char>
    struct node {
        K id {0};
        std::unordered_map<C,K> _map;
        node(){ id = 0;}
        node(const K& _id){ id = _id;}
        ~node(){ id = 0;}
        void print(){
            std::cout<<"node-id:"<<id<<"\n\t";
            std::cout<<"map:{";
            for (const auto &item : _map) {
                std::cout<<item.first<<"->"<<item.second<<",";
            }
            std::cout<<"}\n"<<std::endl;
        }
    };

    template <typename T,typename K,typename C>
    const node<K,C>* init(  T& trie ){
        if(trie.size() == 0)
            trie.push_back(new node<K,C>(0));
        return trie[0];
    }
    template <typename T,typename K,typename C>
    const node<K,C>* root(  T& trie ){
        return trie[0];
    }

    template <typename T>
    void destroy(  T& trie ){
        for (int i = 0; i < trie.size() ; ++i) {
            if(trie[i] != nullptr)
            {
                delete trie[i];
                trie[i] = nullptr;
            }
        }
        trie.clear();
    }

    template <typename T,typename K,typename C>
    const node<K,C>* insert( const K& k,  const C &c, T& trie){
        auto it = trie[k]->_map.find(c);
        if(it == trie[k]->_map.end()){
            K id = (K) trie.size();
            trie[k]->_map[c] = id;
            trie.push_back(new node<K,C>(id));
            return trie[id];
        }
        return trie[trie[k]->_map[c]];
    }

    template <typename T,typename K,typename C>
    const node<K,C>* feed( const K& k,  const  C &c, T& trie ){
            auto it = trie[k]->_map.find(c);
            if(it == trie[k]->_map.end())
                return nullptr;
            return trie[trie[k]->_map[c]];
    }

    template <typename T,typename K,typename C>
    bool isLeaf(const K& k, T& trie ){
        return trie[k]->_map.empty();
    }


    template <typename T>
    void print( T& trie ){
        for (uint32_t i = 0; i < trie.size() ; ++i) {
            trie[i]->print();
        }
    }

    typedef std::vector<node<uint64_t,char>*> c_trie64 ;
    typedef std::vector<node<uint64_t,unsigned char>*> uc_trie64 ;
    typedef std::vector<node<uint32_t,uint32_t>*> ui32_trie32 ;

}


#endif //RRPAIR_RR_TRIE_HPP
