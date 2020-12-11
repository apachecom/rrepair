//
// Created by alejandro on 28-11-19.
//

#ifndef LCP_Z_FAST_TRIE_H
#define LCP_Z_FAST_TRIE_H

#include <utility>
#include <cstdint>
#include <gmpxx.h>
#include <vector>
#include <sdsl/int_vector.hpp>
#include "../node.h"
#include "../dfuds_tree.h"
#include "../../utils.h"

namespace trees{

    template <

            typename Key,
            typename Value,
            typename HashTable,
            typename TopTree = trees::dfuds_tree<>
    >
    class z_fast_trie {

        public:

            z_fast_trie() = default;
            z_fast_trie( const TopTree& _T, const HashTable& _H ){
                _tree = _T;
                _hash = _H;
            }

            z_fast_trie<Key,Value,HashTable,TopTree>& operator[](const z_fast_trie<Key,Value,HashTable,TopTree>& T){

                _tree = T._tree;
                _hash = T._hash;

                return *this;
            }

            ~z_fast_trie() = default;
            /***
             * @return the leaf range corresponding to the maximun prefix comun node
             */
                //todo implementar esto
            std::pair<uint64_t,uint64_t> locus(const Key&s ){
                Value locus = 0;
                binary_search(s,locus);
//                std::cout<<locus<<std::endl;
                uint64_t l = _tree.leafrank(_tree.preorderselect(locus));
                uint64_t ln = _tree.leafnum(_tree.preorderselect(locus));
                return std::make_pair(l,l+ln-1);
            };

            size_t size_in_bytes() const {
                return _tree.size_in_bytes() + _hash.size_in_bytes();
            }
//#ifdef PRINT_LOGS
             void print(){
                 _tree.print();
                     std::cout<<"HASH TABLE\n";

                 auto it = _hash.begin();
                 for (int i = 0; i < _hash.size() ; ++i) {
                     std::cout<<"string:"<<it->first<<", preorder:"<<it->second<<std::endl;
                     ++it;
                 }
             }
//#endif


        protected:

            /***
             * this function compute the binary search in the z-fast-trie given a key
             * @param _key
             * @return
             */
            uint64_t binary_search(const Key& _key, Value& locus)
            {
                uint64_t i = 0, j = _key.size();
                uint64_t m = 0;
//                std::cout<<"HOLA"<<std::endl;
                while(i <= j){
                    m = lcg::utils::twoFattestNumber(i,j);
                    Key ss;ss.resize(m);
                    std::copy(_key.begin(),_key.begin()+m,ss.begin());
//                    std::cout<<"i:"<<i<<" m:"<<m<<"j:"<<j<<std::endl;
//                    std::cout<<"key:"<<ss<<std::endl;
                    if ( _hash.find(ss) == _hash.end() ){
                        //not found
//                        std::cout<<"\tnot found"<<std::endl;

                        j = m - 1;
                    }else{
                        //found
//                        std::cout<<"\tfound"<<std::endl;

                        i = m + 1;
                        locus = _hash[ss];
//                        std::cout<<"\tpreorder"<<locus<<std::endl;

                    }
                }
                return m;
            }

            TopTree _tree;
            HashTable _hash;

        };



    template <
            typename Tree,
            typename Key,
            typename Value,
            typename HashTable,
            typename TopTree = trees::dfuds_tree<>,
            typename node_type = uint64_t
            >
        class lz_fast_trie_builder{

            public:
                lz_fast_trie_builder() = default;
                ~lz_fast_trie_builder() = default;

                z_fast_trie<Key,Value,HashTable,TopTree> operator()(const Tree& T){

                    auto root = T.root();
                    HashTable H;

                    std::vector<uint32_t> ch;
                    uint64_t n_nodes = 0,preorder = 0;
                    T.dfs(root,[&T,&H,&ch,&n_nodes,&preorder](const node *n, const char &a, const ulong &j, const ulong &off, const ulong & pre_off){
                        /**
                         * Compute hash values for the nodes
                         * */
                        auto s = T[n->ptr_leaf];
//                        std::cout<<s.toString()<<std::endl;

                        uint64_t mm = lcg::utils::twoFattestNumber(pre_off+1,off);
                        Key _k;
                        _k.resize(mm);
                        std::copy(s.begin(),s.begin()+mm,_k.begin());
//                        std::cout<<"s:"<<_k<<" range:("<<pre_off<<","<<off<<"] 2fn:"<<mm<<std::endl;
//                        H.insert(_k,(Value)preorder);
                        ++preorder;
                        H[_k] = (Value)preorder;
                        /**
                         * Store number of childrens for dfuds building
                         * */
                        if(n->childs.empty() && n->ids.size() > 1){
                            //  multiples strings in the Trie
                            //  add a virtual node
                            ch.push_back(n->ids.size());
                            //  add m leaves
                            for (ulong i = 0; i < n->ids.size(); ++i) {
                                ch.push_back(0);
                            }

                            n_nodes += n->ids.size() + 1;

                        }else{
                            ++n_nodes;
                            ch.push_back(n->childs.size());
                        }

                        return true;
                    },'_',0,0,0);

                    sdsl::bit_vector dfuds_bv (2*n_nodes+2,1);
                    uint64_t off = 3;
                    dfuds_bv[2] = 0;

                    for (const auto &item : ch) {
                        dfuds_bv[off+item] = 0;
                        off += item+1;
                    }

                    TopTree tree;
                    tree.build(dfuds_bv);
                    z_fast_trie < Key,Value,HashTable,TopTree >  Z(tree,H);
                    return Z;
                }

        };











}




#endif //LCP_Z_FAST_TRIE_H
