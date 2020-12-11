//
// Created by inspironXV on 8/17/2018.
//

#ifndef IMPROVED_GRAMMAR_INDEX_BP_TREE_H
#define IMPROVED_GRAMMAR_INDEX_BP_TREE_H


#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "node.h"

namespace trees{

    template <
            typename bv = sdsl::bit_vector,
            typename bv_rank_10 = sdsl::rank_support_v<10,2>,
            typename bv_select_10 = sdsl::select_support_mcl<10,2>,
            typename bv_select_0 = sdsl::bit_vector::select_0_type,
            typename parenthesis_seq = sdsl::bp_support_sada<>,
            typename bp_long = uint64_t

            >
    class bp_tree {

    public:
        bv bit_vector;
        parenthesis_seq bps;
        bv_rank_10 rank_10;
        bv_select_10 select_10;
        bv_select_0 select_0;


    public:
        bp_tree() = default;
        ~bp_tree() = default;

        void build( const bv & );

        static inline short root(){ return 0; }
        /*
         * return the number of nodes in the subtree of the node
         * */
        inline bp_long subtree(const bp_long& v)const{
            return (bps.find_close(v)-v+1)/2;
        }

        inline bool isleaf(const bp_long & v)const{
            return !bit_vector[v];
        }
        /*
         * (preoreder) return the preorder of a node
         * */
        inline bp_long pre_order( const bp_long& v)const{
            return bps.rank(v);
        }
        /*
         * (preoreder select) return the i-th node in preorder
         * */
        inline bp_long operator[](const bp_long &  i)const{
            return bps.select(i);
        };
        /*
         *  return the i-th child of a node
         * */
        bp_long child(const bp_long& , const bp_long& )const;

        /*
         * return the rank of a node between its brothers
         *
         *  bp_long childrank(const bp_long & )const;
         */

         /*
         * return the number of children of a node
         * */
        bp_long children(const bp_long &v)const;
        /*
         * return the number of leaves in the subtree of a node
         * */
        inline bp_long leafnum(const bp_long & v)const {
            return leafrank(bps.find_close(v))-leafrank(v);
        }
        /*
         * return the rank between the leaf of the left most leaf of a node
         * */
        inline bp_long leafrank(const bp_long & v)const{
            return rank_10(v)+1;
        };
        /*
         * return the i-th leaf
         * */
        inline bp_long leafselect(const bp_long &i)const {
            return select_10(i);
        }
        /*
         * return the parent of a node
         * */
        inline bp_long parent(const bp_long& v)const{
            if(v == 0) return bps.size();
            return bps.enclose(v);
        }

        inline bp_long fchild(const bp_long& v) const{
            return v+1;
        }
        inline bp_long lchild(const bp_long& v )const{
            return bps.find_open(bps.find_close(v)-1);
        }

        inline bp_long nsibling(const bp_long& v)const{
            auto nb = bps.find_close(v)+1;
            if(bit_vector[nb] == 0) return bps.size();
            return nb;
        }

        inline bp_long psibling(const bp_long& v)const{
            if(bit_vector[v-1] == true) return bps.size();
            return  bps.find_open(v-1);
        }

        inline bp_long levelancestor(const bp_long& v, const int& d) const{
            return bps.level_anc(v,d);
        }

        size_t size_in_bytes() const;


#ifdef DEBUG_PRINT
        void print_size_in_bytes(const std::string &) const;
#endif

        void save(std::fstream &f) const;

        void load(std::fstream &f);

        inline bp_long depth(const bp_long& v) const{
            return 2*bps.rank(v)-(v+1);
        }



        /*
         * dfs over the tree
         * */
        template <typename K>
        void dfs_preorder(const bp_long &node, const K &f){

            auto keep = f(node,this);
            if(!keep)
                return;
            bp_long n  = children(node);
            for (bp_long i = 0; i <  n ; ++i) {
                dfs_preorder(child(node,i+1),f);
            }
        }

    };

    template<typename bv, typename bv_rank_10, typename bv_select_10, typename bv_select_0, typename parenthesis_seq, typename bp_long>
    bp_long bp_tree<bv, bv_rank_10, bv_select_10, bv_select_0, parenthesis_seq, bp_long>::child(const bp_long & v,
                                                                                                const bp_long & t) const {

            assert(bit_vector[v+1] == 0);
            if(bit_vector[v+1]==0) return 0;
            if(t == 1) return  v+1;

            auto l = v+2;
            auto r = bps.find_close(v);

            for(int i = 2; i <= t ; ++i)
            {
                auto next = bps.rmq(l,r);
                if(next+1 >= r) return bit_vector.size();
                l = next+1;
            }

            return l;
    }

//    template<typename bv, typename bv_rank_10, typename bv_select_10, typename bv_select_0, typename parenthesis_seq, typename bp_long>
//    bp_long
//    bp_tree<bv, bv_rank_10, bv_select_10, bv_select_0, parenthesis_seq, bp_long>::childrank(const bp_long &) const {
//        return nullptr;
//    }

    template<typename bv, typename bv_rank_10, typename bv_select_10, typename bv_select_0, typename parenthesis_seq, typename bp_long>
    bp_long
    bp_tree<bv, bv_rank_10, bv_select_10, bv_select_0, parenthesis_seq, bp_long>::children(const bp_long &v) const {

            if(bit_vector[v+1] == 0) return 0;

            auto l = v+1;
            bp_long ch = 0;
            while(l != bps.size())
            {   ch++;
                l = nsibling(l);
            }

            return ch;
    }

    template<typename bv, typename bv_rank_10, typename bv_select_10, typename bv_select_0, typename parenthesis_seq, typename bp_long>
    size_t bp_tree<bv, bv_rank_10, bv_select_10, bv_select_0, parenthesis_seq, bp_long>::size_in_bytes() const {
        return  sdsl::size_in_bytes(bit_vector) +
                sdsl::size_in_bytes(bps)        +
                sdsl::size_in_bytes(select_10)  +
                sdsl::size_in_bytes(rank_10)    +
                sdsl::size_in_bytes(select_0)
                ;

    }

    template<typename bv, typename bv_rank_10, typename bv_select_10, typename bv_select_0, typename parenthesis_seq, typename bp_long>
    void bp_tree<bv, bv_rank_10, bv_select_10, bv_select_0, parenthesis_seq, bp_long>::save(std::fstream &f) const {

        sdsl::serialize(bit_vector,f);
    }

    template<typename bv, typename bv_rank_10, typename bv_select_10, typename bv_select_0, typename parenthesis_seq, typename bp_long>
    void bp_tree<bv, bv_rank_10, bv_select_10, bv_select_0, parenthesis_seq, bp_long>::load(std::fstream &f) {
        sdsl::load(bit_vector,f);
        bps =   sdsl::bp_support_sada<>(&bit_vector);
        rank_10 = sdsl::rank_support_v<10,2> (&bit_vector);
        select_10 = sdsl::select_support_mcl<10,2>(&bit_vector);
        select_0  = typename bv::select_0_type(&bit_vector);

    }

    template<typename bv, typename bv_rank_10, typename bv_select_10, typename bv_select_0, typename parenthesis_seq, typename bp_long>
    void bp_tree<bv, bv_rank_10, bv_select_10, bv_select_0, parenthesis_seq, bp_long>::build(const bv & v) {

        bit_vector = bv(v);
        bps =   sdsl::bp_support_sada<>(&bit_vector);
        rank_10 = sdsl::rank_support_v<10,2> (&bit_vector);
        select_10 = sdsl::select_support_mcl<10,2>(&bit_vector);
        select_0  = typename bv::select_0_type(&bit_vector);

    }
#ifdef DEBUG_PRINT
    template<typename bv, typename bv_rank_10, typename bv_select_10, typename bv_select_0, typename parenthesis_seq, typename bp_long>
    void bp_tree<bv, bv_rank_10, bv_select_10, bv_select_0, parenthesis_seq, bp_long>::print_size_in_bytes(
            const std::string & s) const {

            std::cout<<s+"tree bitvector \t"<<sdsl::size_in_mega_bytes(bit_vector)<<"(bytes)"<<std::endl;
            std::cout<<s+"\t tree bitvector length\t"<<bit_vector.size()<<std::endl;
            std::cout<<s+"balanced parentheses sequence \t"<<sdsl::size_in_mega_bytes(bps)       <<"(bytes)"<<std::endl;
            std::cout<<s+"\t balanced parentheses sequence \t"<<bps.size()<<std::endl;
            std::cout<<s+"tree bitvector(select_00) \t"<<sdsl::size_in_mega_bytes(select_10) <<"(bytes)"<<std::endl;
            std::cout<<s+"tree bitvector(select_0) \t"<<sdsl::size_in_mega_bytes(select_0)  <<"(bytes)"<<std::endl;
            std::cout<<s+"tree bitvector(rank_00) \t"<<sdsl::size_in_mega_bytes(rank_10)   <<"(bytes)"<<std::endl;

    }

#endif


    template <
            typename Tree,
            typename bv = sdsl::bit_vector,
            typename bv_rank_10 = sdsl::rank_support_v<10,2>,
            typename bv_select_10 = sdsl::rank_support_v<10,2>,
            typename bv_select_0 = sdsl::rank_support_v<10,2>,
            typename parenthesis_seq = sdsl::bp_support_sada<>,
            typename bp_long = uint64_t
    > class bp_tree_builder {


        bp_tree_builder() = default;
        ~bp_tree_builder() = default;


            bp_tree<bv, bv_rank_10, bv_select_10, bv_select_0, parenthesis_seq, bp_long> operator ()(const Tree& T) const {

            /**
             * node
             * a is the simbol in the edge
             * j is the jump in the edge
             * */

            auto root = T.root();
            std::vector<bool> bit_v;

            T.dfs(root,
                  [&bit_v](const node *n, const char &a, const ulong &j)
                  {

                        bit_v.push_back(1);

                        if(n->childs.empty() && !n->ids.empty()){
                              //  multiples strings in the Trie
                              //  add m leaves
                              for (ulong i = 0; i < n->ids.size(); ++i) {
                                  bit_v.push_back(1);
                                  bit_v.push_back(0);
                              }
                        }

                    },
                  [&bit_v](const node *n, const char &a, const ulong &j)
                  {
                      bit_v.push_back(0);

                  },
                  '_',0);

            bv bp_tree_v(bit_v.size(),0);
            for (uint32_t i = 0; i < bit_v.size();++i) {
                if(bit_v[i])bp_tree_v[i] = 1;
            }
            bit_v.clear();
            bp_tree<bv, bv_rank_10, bv_select_10, bv_select_0, parenthesis_seq, bp_long> tree;
            tree.build(bp_tree_v);
            return tree;

        }
    };


}

#endif //IMPROVED_GRAMMAR_INDEX_BP_TREE_H
