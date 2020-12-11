//
// Created by via56 on 22-09-20.
//

#ifndef LCP_LOUD_TREE_H
#define LCP_LOUD_TREE_H

#include <sdsl/int_vector.hpp>
#include <sdsl/bit_vectors.hpp>
#include "node.h"

namespace trees{

    template <
            typename bv = sdsl::bit_vector,
            typename bv_rank = sdsl::bit_vector::rank_1_type,
            typename bv_select = sdsl::bit_vector::select_1_type,
            typename bv_select_0 = sdsl::bit_vector::select_0_type,
            typename node_type = uint64_t
            >
    class loud_tree {



        public:

            loud_tree() = default;
            ~loud_tree() = default;

            inline void build(const bv&);
            /**
             * read from file
             * */
//            inline void build(const std::string&);

            inline node_type root()const;

//            inline node_type fchild(const node_type&)const;

//            inline node_type lchild(const node_type&)const;

            inline node_type nsibling(const node_type&)const;

            inline node_type psibling(const node_type&)const;

            inline node_type parent(const node_type&)const;

            inline bool isleaf(const node_type&)const;
            /**
             * preorder of a node
             * */
            inline uint64_t nodemap(const node_type&)const;
            /**
             *  node of preorder i
             * */
            inline node_type nodeselect(const uint64_t & i)const;

            inline uint64_t  children(const node_type&)const;

            inline node_type child(const node_type&, const uint64_t & )const;

            inline uint64_t childrank(const node_type&)const;

            inline node_type lca(const node_type&,const node_type&) const;

            inline size_t size_in_bytes() const;

            void print(){

                std::cout<<"vector:";
                for (int i = 0; i < _vector.size() ; ++i) {
                    std::cout<<_vector[i];
                }
                std::cout<<std::endl;

            }

    protected:
            bv _vector;
            bv_rank rank_vector;
            bv_select select_vector;
            bv_select_0 select0_vector;

            inline node_type succ_0(const node_type& ) const;
            inline node_type pred_0(const node_type& ) const;
            inline node_type rank_0_vector(const node_type&) const;
            inline node_type edge(const node_type&) const;

    };


    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    node_type loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::edge(const node_type & node) const {
        assert(node != root());
        return select_vector(rank_0_vector(node));;
    }

    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    node_type loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::succ_0(const node_type & node) const {
        return select0_vector( rank_0_vector(node) + 1);
    }

    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    node_type loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::pred_0(const node_type &node) const {
        return select0_vector(rank_0_vector(node)) ;
    }

    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    node_type loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::rank_0_vector(const node_type & node) const {
        return node - rank_vector(node);
    }

    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    node_type loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::root() const {
        return 2;
    }
    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    node_type
    loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::child(const node_type & node, const uint64_t &i) const {
        return select0_vector( rank_vector(node + i) ) + 1;
    }

    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    uint64_t loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::children(const node_type& node) const {
        return succ_0(node)-node;
    }

    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    uint64_t loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::childrank(const node_type & node) const {
        if(node == root()) return 1;
        node_type e = edge(node);
        return e - pred_0(e);
    }

    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    node_type loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::nodeselect(const uint64_t &i) const {
        return select0_vector(i)+1;
    }

    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    uint64_t loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::nodemap(const node_type & node) const {
        return rank_0_vector(node);
    }

    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    bool loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::isleaf(const node_type & node) const {
        return !_vector[node];
    }

    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    node_type loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::parent(const node_type & node) const {
        if(node == root()) return 0;
        node_type e = edge(node);
        return pred_0(e) + 1;
    }

    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    node_type loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::psibling(const node_type &node) const {

        return node == root() ? 0: pred_0(node - 1) + 1;
    }

    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    node_type loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::nsibling(const node_type & node) const {
        return succ_0(node) + 1;
    }

    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    node_type loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::lca(const node_type& u, const node_type &v) const {
        while (u != v)
            if(u > v)
                u = parent(u);
            else
                v = parent(v);

        return u;
    }

    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    void loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::build(const bv & vector) {
        _vector = vector;
        rank_vector = bv_rank(&_vector);
        select_vector = bv_select(&_vector);
        select0_vector = bv_select_0(&_vector);
    }

    template<typename bv, typename bv_rank, typename bv_select, typename bv_select_0, typename node_type>
    size_t loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type>::size_in_bytes() const {
        return  sdsl::size_in_bytes(_vector) +
                sdsl::size_in_bytes(rank_vector) +
                sdsl::size_in_bytes(select_vector) +
                sdsl::size_in_bytes(select0_vector);
    }



    template <
                typename Tree,
                typename bv = sdsl::bit_vector,
                typename bv_rank = sdsl::bit_vector::rank_1_type,
                typename bv_select = sdsl::bit_vector::select_1_type,
                typename bv_select_0 = sdsl::bit_vector::select_0_type,
                typename node_type = uint64_t
                >
    class loud_builder {

        public:
                    loud_builder() = default;
                    ~loud_builder() = default;

                    loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type> operator ()(const Tree& T) const{

                        /**
                         * node
                         * a is the simbol in the edge
                         * j is the jump in the edge
                         * */

                        auto root = T.root();
                        std::vector<node_type> ch;
                        uint64_t n_nodes = 0;

                        T.bfs(root,[&ch,&n_nodes](const node *n, const char &a, const ulong &j){

//                            std::cout<<"id"<<n->id<<std::endl;
//                            std::cout<<"nchilds"<<n->childs.size()<<std::endl;
//                            std::cout<<"-------------------"<<std::endl;
                            if(n->childs.empty() && n->ids.size() > 1){
//                                std::cout<<"AQUI NO DEBE ENTRAR!!\n"<<"ids:"<<n->id<<":"<<n->ids.front()<<std::endl;

                                //  multiples strings in the Trie
                                //  add a virtual node
                                ch.push_back(n->ids.size());
                                //  add m leaves
                                for (ulong i = 0; i < n->ids.size(); ++i) {
                                    ch.push_back(0);
                                }

                                n_nodes += n->ids.size() + 1;

                            }else{
                                    ch.push_back(n->childs.size());
                                    ++n_nodes;
                            }

                            return true;

                        },'_',0);


                        bv loud_bv (2*n_nodes+1,1);
                        uint64_t off = 2;
                        loud_bv[1] = 0;

                        for (const auto &item : ch) {
                            loud_bv[off+item] = 0;
                            off += item+1;
                        }
                        loud_tree<bv, bv_rank, bv_select, bv_select_0, node_type> l_tree;
                        l_tree.build(loud_bv);
                        return l_tree;

                    }




    };








}

#endif //LCP_LOUD_TREE_H
