//
// Created by inspironXV on 8/17/2018.
//

#ifndef IMPROVED_GRAMMAR_INDEX_DFUDS_TREE_H
#define IMPROVED_GRAMMAR_INDEX_DFUDS_TREE_H


#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include <sdsl/rrr_vector.hpp>
#include "node.h"
namespace trees {



    template <
            typename bv = sdsl::bit_vector,
            typename bv_rank_00 = sdsl::rank_support_v<00, 2>,
            typename bv_select_00 = sdsl::select_support_mcl<00, 2>,
            typename bv_select_0 = sdsl::bit_vector::select_0_type,
            typename parenthesis_seq = sdsl::bp_support_sada<> ,
            typename dfuds_long = uint64_t
            >
    class dfuds_tree {

    public:

        bv bit_vector;
        parenthesis_seq bps;
        bv_rank_00 rank_00;
        bv_select_00 select_00;
        bv_select_0 select_0;

        inline dfuds_long succ_0(const dfuds_long& ) const;
        inline dfuds_long pred_0(const dfuds_long& ) const;
        inline dfuds_long rank_0_vector(const dfuds_long&) const;

//    public:
        dfuds_tree() = default;

        ~dfuds_tree() = default;

        dfuds_tree& operator=(const dfuds_tree &);

        void build(const sdsl::bit_vector &);

        static inline short root(){ return 3;}
        /*
         * return the number of nodes in the subtree of the node
         * */
        inline dfuds_long subtree(const dfuds_long & v)const{
            return ((bps.fwd_excess(v - 1, -1) - v) / 2 + 1);
        }

        inline bool isleaf(const dfuds_long &i)const{
            return !bit_vector[i];
        }

        /*
         * (preoreder) return the preorder of a node
         * */
        inline dfuds_long pre_order(const dfuds_long & v)const{
            return  v-bps.rank(v)+bit_vector[v] ;
        }

        /*
         * (preoreder select) return the i-th node in preorder
         * */
        inline dfuds_long operator[](const dfuds_long & i)const{
            return select_0(i)+1;
        }
        inline dfuds_long preorderselect(const dfuds_long & i)const{
            return select_0(i)+1;
        }
        inline dfuds_long nsibling(const dfuds_long & v)const{

            return bps.fwd_excess(v-1,-1)+1;
        }

        inline dfuds_long psibling(const dfuds_long& v) const {
            return bps.find_close(bps.find_open(v - 1) + 1) + 1;

        }

        inline dfuds_long lchild(const dfuds_long & v)const{
            if(bit_vector[v] == false) return 0;
            return bps.find_close(v)+1;
        }
        inline dfuds_long fchild(const dfuds_long & v)const{
            if(bit_vector[v] == false) return 0;
            return succ0(v)+1;
        }


        /*
         *  return the i-th child of a node
         * */
        inline dfuds_long child(const dfuds_long & v, const dfuds_long & t)const{
            if(!bit_vector[v] /*|| t > children(v)*/) return 0;

            size_t close = bps.find_close(succ0(v)-t);

            if( close == bps.size())return 0;

            return close+1;
        }

        /*
         * return the rank of a node between its brothers
         * */
        inline dfuds_long childrank(const dfuds_long & v)const{
            //if(v == 3) return 0;

            size_t bp_parent = bps.find_open(v-1);

            return succ0(bp_parent) - bp_parent;

        }

        /*
         * return the number of children of a node
         * */
        inline dfuds_long children(const dfuds_long &v)const{
            if(!bit_vector[v]) return  0;

            size_t zeros = v - bps.rank(v)+1 ;
            return select_0(zeros+1) - v;
        }

        /*
         * return the number of leaves in the subtree of a node
         * */
        inline dfuds_long leafnum(const dfuds_long & v)const{
            return leafrank( bps.fwd_excess(v-1,-1) +1)-leafrank(v);
        }

        /*
         * return the rank between the leaf of the left most leaf of a node
         * */
        inline dfuds_long leafrank(const dfuds_long & v) const{
            return rank_00(v)+1;
        }

        inline dfuds_long nextTreeNode(const dfuds_long & v) const{ return bps.fwd_excess(v-1,-1)+1; }

        inline dfuds_long lastleaf(const dfuds_long & v)const{ return leafrank( bps.fwd_excess(v-1,-1) +1) - 1;}
        /*
         * return the i-th leaf
         * */
        inline dfuds_long leafselect(const dfuds_long & i)const{
            return select_00(i);

        }

        /*
         * return the parent of a node
         * */
        inline dfuds_long parent(const dfuds_long & v)const{
            if(v == 3) return 0;
            /*auto p = bps.find_open(v-1);
            while(bit_vector[p-1]!=0)--p;
            return p;*/
            return pred0( bps.find_open(v-1) )+1;

        }

        inline bool is_ancestor(const dfuds_long &u, const dfuds_long &v)const{
            return (u < v) && (v < bps.fwd_excess(u-1,-1));
        }

        /*
         * dfs preorder over the tree
         * */
        template<typename K>
        void dfs_preorder(const dfuds_long &node, const K &f) const{

            auto keep = f(node, this);
            if (!keep)
                return;
            dfuds_long n = children(node);
            for (dfuds_long i = 0; i < n; ++i) {
                dfs_preorder(child(node, i + 1), f);
            }
        }

        /*
         * dfs posorder over the tree
         *
         * */
        template<typename K>
        void dfs_posorder(const dfuds_long &node, const K &f)const {

            dfuds_long n = children(node);
            for (dfuds_long i = 0; i < n; ++i) {
                dfs_posorder(child(node, i + 1), f);
            }

            f(node);
        }

        /*
         * this function walks a path from root to a leaf
         *
         * */
        template<typename K>
        void path(dfuds_long &node, const K &f) const{
            dfuds_long next_node = 0;

            if (f(node, next_node))
                path(next_node, f);
        }

        template<typename K>
        uint find_child_dbs(const uint & node,uint & ls,uint & hs,const K &f)const{

            uint p2 = 1;
            /*
             * f return true if p2 < value else return false             *
             * */
            while( f(p2) ){

                if(p2*2 > hs){
                    p2 = hs;
                    break;
                }
                p2*=2;
            }

            ls = (p2 == 1)?1:p2/2; hs = p2;

            while(ls+1 < hs)
            {
                uint m = (ls + hs)/2;
                f(m) ?  (ls = m):(hs = m-1);
            }

            if (f(hs) == true)
                return hs;
            return ls;

        }

        template<typename K>
        uint find_child_dbs_mirror(const uint & node,uint & ls,uint & hs,const K &f)const{

            uint p2 = hs;
            /*
             * f return true if p2 < value else return false             *
             * */

            while( p2 && f(p2) ){
                p2/=2;
            }


            ls = p2 ? p2 : 1;
            if(hs/2 != ls)
                hs = (p2*2 < hs)?p2*2:hs;


            while(ls+1 < hs)
            {
                uint m = (ls + hs)/2;
                f(m) ? (hs = m) : (ls = m+1);
            }

            if (f(ls) == false)
                return hs;
            return ls;

        }


        template<typename K>
        dfuds_long
        find_child(const dfuds_long &node, dfuds_long &ls, dfuds_long &hs, const K &f) const{

            while(ls+1 < hs){
                dfuds_long rank_ch = (ls + hs) / 2;
                //f(child(node, rank_ch)) ? (hs = rank_ch-1) : (ls = rank_ch);
                f(rank_ch) ? (hs = rank_ch-1) : (ls = rank_ch);
            }

            //if (ls+1 >= hs )
            //{


                //if (f(child(node, hs)) == true)
                if (f(hs) == true)
                    return ls;
                return hs;
            //}

            //dfuds_long rank_ch = (ls + hs) / 2;
            /*
             * If value < value(node) f must return true
             * */
            //f(child(node, rank_ch)) ? (hs = rank_ch-1) : (ls = rank_ch);

            //return find_child(node, ls, hs, f);
        }


        size_t size_in_bytes() const;
        void save(std::fstream &f) const;
        void load(std::fstream &f);
#ifdef DEBUG_PRINT
        void print_size_in_bytes(const std::string &) const;
#endif
        inline dfuds_long rank_1(const dfuds_long &i) const{
            return bps.rank(i);
        }


        void print()const;


    public:
        inline size_t pred0(const dfuds_long &i) const{
            if(bit_vector[i]==0)
                return i;
            return select_0(i - bps.rank(i)+1);
        }

        inline size_t succ0(const dfuds_long &i) const{
            return select_0(i - bps.rank(i)+1+bit_vector[i]);
        }

    };

    template<typename bv, typename bv_rank_00, typename bv_select_00, typename bv_select_0, typename parenthesis_seq, typename dfuds_long>
    void dfuds_tree<bv, bv_rank_00, bv_select_00, bv_select_0, parenthesis_seq, dfuds_long>::build(
            const sdsl::bit_vector &_bv) {

        bit_vector = bv(_bv);
        bps =   sdsl::bp_support_sada<>(&bit_vector);
        rank_00 = sdsl::rank_support_v<00,2> (&bit_vector);
        select_00 = sdsl::select_support_mcl<00,2>(&bit_vector);
        select_0  = bv_select_0(&bit_vector);

    }

    template<typename bv, typename bv_rank_00, typename bv_select_00, typename bv_select_0, typename parenthesis_seq, typename dfuds_long>
    size_t dfuds_tree<bv, bv_rank_00, bv_select_00, bv_select_0, parenthesis_seq, dfuds_long>::size_in_bytes() const {

        return  sdsl::size_in_bytes(bit_vector) +
            sdsl::size_in_bytes(bps)        +
            sdsl::size_in_bytes(select_00)  +
            sdsl::size_in_bytes(select_0)   +
            sdsl::size_in_bytes(rank_00)
            ;
    }
#ifdef DEBUG_PRINT
    template<typename bv, typename bv_rank_00, typename bv_select_00, typename bv_select_0, typename parenthesis_seq, typename dfuds_long>
    void dfuds_tree<bv, bv_rank_00, bv_select_00, bv_select_0, parenthesis_seq, dfuds_long>::print_size_in_bytes(
            const std::string & s) const {

        std::cout<<s+"tree bitvector \t"<<sdsl::size_in_mega_bytes(bit_vector)<<"(bytes)"<<std::endl;
        std::cout<<s+"\t tree bitvector length\t"<<bit_vector.size()<<std::endl;
        std::cout<<s+"balanced parentheses sequence \t"<<sdsl::size_in_mega_bytes(bps)       <<"(bytes)"<<std::endl;
        std::cout<<s+"\t balanced parentheses sequence \t"<<bps.size()<<std::endl;
        std::cout<<s+"tree bitvector(select_00) \t"<<sdsl::size_in_mega_bytes(select_00) <<"(bytes)"<<std::endl;
        std::cout<<s+"tree bitvector(select_0) \t"<<sdsl::size_in_mega_bytes(select_0)  <<"(bytes)"<<std::endl;
        std::cout<<s+"tree bitvector(rank_00) \t"<<sdsl::size_in_mega_bytes(rank_00)   <<"(bytes)"<<std::endl;


    }
#endif
    template<typename bv, typename bv_rank_00, typename bv_select_00, typename bv_select_0, typename parenthesis_seq, typename dfuds_long>
    void
    dfuds_tree<bv, bv_rank_00, bv_select_00, bv_select_0, parenthesis_seq, dfuds_long>::save(std::fstream &f) const {

            sdsl::serialize(bit_vector,f);
    }

    template<typename bv, typename bv_rank_00, typename bv_select_00, typename bv_select_0, typename parenthesis_seq, typename dfuds_long>
    void dfuds_tree<bv, bv_rank_00, bv_select_00, bv_select_0, parenthesis_seq, dfuds_long>::load(std::fstream &f) {

            sdsl::load(bit_vector,f);
            bps =   sdsl::bp_support_sada<>(&bit_vector);
            rank_00 = sdsl::rank_support_v<00,2> (&bit_vector);
            select_00 = sdsl::select_support_mcl<00,2>(&bit_vector);
            select_0  = typename bv::select_0_type(&bit_vector);
    }
//#ifdef DEBUG_PRINT
    template<typename bv, typename bv_rank_00, typename bv_select_00, typename bv_select_0, typename parenthesis_seq, typename dfuds_long>
    void dfuds_tree<bv, bv_rank_00, bv_select_00, bv_select_0, parenthesis_seq, dfuds_long>::print() const {

        std::cout<<"vector:";
        for (int i = 0; i < bit_vector.size() ; ++i) {
            std::cout<<bit_vector[i];
        }
        std::cout<<std::endl;
    }
//#endif
    template <
            typename bv,
            typename bv_rank_00,
            typename bv_select_00,
            typename bv_select_0,
            typename parenthesis_seq,
            typename dfuds_long
            >
    dfuds_tree<bv, bv_rank_00, bv_select_00, bv_select_0, parenthesis_seq, dfuds_long> &
    dfuds_tree<bv, bv_rank_00, bv_select_00, bv_select_0, parenthesis_seq, dfuds_long>::operator=(const dfuds_tree<bv, bv_rank_00, bv_select_00, bv_select_0, parenthesis_seq, dfuds_long> &T) {

        bit_vector = T.bit_vector;
        bps =   sdsl::bp_support_sada<>(&bit_vector);
        rank_00 = sdsl::rank_support_v<00,2> (&bit_vector);
        select_00 = sdsl::select_support_mcl<00,2>(&bit_vector);
        select_0  = typename bv::select_0_type(&bit_vector);

        return *this;
    }

    template<typename bv, typename bv_rank_00, typename bv_select_00, typename bv_select_0, typename parenthesis_seq, typename dfuds_long>
    dfuds_long dfuds_tree<bv, bv_rank_00, bv_select_00, bv_select_0, parenthesis_seq, dfuds_long>::pred_0(
            const dfuds_long & node) const {
        return select_0( rank_0_vector(node) + 1);
    }

    template<typename bv, typename bv_rank_00, typename bv_select_00, typename bv_select_0, typename parenthesis_seq, typename dfuds_long>
    dfuds_long dfuds_tree<bv, bv_rank_00, bv_select_00, bv_select_0, parenthesis_seq, dfuds_long>::succ_0(
            const dfuds_long & node) const {
        return select_0(rank_0_vector(node)) ;
    }

    template<typename bv, typename bv_rank_00, typename bv_select_00, typename bv_select_0, typename parenthesis_seq, typename dfuds_long>
    dfuds_long dfuds_tree<bv, bv_rank_00, bv_select_00, bv_select_0, parenthesis_seq, dfuds_long>::rank_0_vector(
            const dfuds_long &node) const {
        return node - bps.rank(node);
    }


    template <
            typename Tree,
            typename bv = sdsl::bit_vector,
            typename bv_rank_00 = sdsl::rank_support_v<00, 2>,
            typename bv_select_00 = sdsl::select_support_mcl<00, 2>,
            typename bv_select_0 = sdsl::bit_vector::select_0_type,
            typename parenthesis_seq = sdsl::bp_support_sada<> ,
            typename dfuds_long = uint64_t
            >

            class dfuds_builder{

                public:
                    dfuds_builder() = default;
                    ~dfuds_builder() = default;

                    dfuds_tree<bv, bv_rank_00, bv_select_00, bv_select_0, parenthesis_seq, dfuds_long> operator ()(const Tree& T) const
                    {

                        /**
                         * node
                         * a is the simbol in the edge
                         * j is the jump in the edge
                         * */

                        auto root = T.root();
                        std::vector<dfuds_long> ch;
                        uint64_t n_nodes = 0;

                        T.dfs(root,[&n_nodes,&ch](const node *n, const char &a, const ulong &j){

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
//                                if(!n->childs.empty())
//                                {
                                    ++n_nodes;
                                    ch.push_back(n->childs.size());

//                                }
                            }

                            return true;

                        },'_',0);

                        bv dfuds_bv (2*n_nodes+2,1);
                        uint64_t off = 3;
                        dfuds_bv[2] = 0;

                        for (const auto &item : ch) {
                            dfuds_bv[off+item] = 0;
                            off += item+1;
                        }

                        dfuds_tree<bv, bv_rank_00, bv_select_00, bv_select_0, parenthesis_seq, dfuds_long> tree;
                        tree.build(dfuds_bv);

                        return tree;
                    }

            };

}

#endif //IMPROVED_GRAMMAR_INDEX_DFUDS_TREE_H
