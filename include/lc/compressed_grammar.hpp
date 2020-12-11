//
// Created by via56 on 06-12-20.
//

#ifndef RRPAIR_COMPRESSED_GRAMMAR_HPP
#define RRPAIR_COMPRESSED_GRAMMAR_HPP

#include <cstdint>
#include <sdsl/int_vector.hpp>
#include <sdsl/sd_vector.hpp>
#include <sdsl/wavelet_trees.hpp>
#include "macros.hpp"
#include "../trees/dfuds_tree.h"
#include <sdsl/inv_perm_support.hpp>

namespace lc{

    typedef sdsl::wt_gmr<
            sdsl::int_vector<>,
            sdsl::inv_multi_perm_support<INV_PERM_X>
    > GolynskiSeq;

    template <typename G>
    uint64_t destroy(const G &grammar);

    template <typename G>
    uint64_t build(const char * text, const uint64_t &len, const G &grammar);

    template <typename G>
    uint64_t expansionLen(const uint64_t &node, const G &grammar);

    /**
     * compare the string suffix with a grammar suffix
     * @params
     * prenode - preorder of the node where the suffix start
     * str - string to compare
     * i - start position of the suffix string to compare
     * */
    template<typename G>
    int cmp_suffix_grammar(const uint64_t &prenode, const std::string &str, const uint64_t &i, const G &grammar);
    template<typename G>
    int cmp_prefix_rule(const uint64_t &prenode, const std::string & str, const uint64_t &i, const G &grammar);

    //  return true if the rule is a terminal simbol
    //  return also the symbol the node X
    template<typename G>
    bool isTerminal(const uint64_t &prenode, uint64_t &X, const G &grammar);

    template<typename G>
    uint64_t next(const uint64_t &prenode, const G &grammar);
    template<typename G>
    uint64_t off(const uint64_t &prenode, const G &grammar);
    template<typename G>
    uint64_t ancs(const uint64_t &prenode, const G &grammar);
    // prenode is the node where the primary occ lies and p is the negative off relative to the partition of the pattern
    // m is the length of the pattern
    template <typename G>
    void findSecondOccPreorderNode(const uint64_t& prenode, const uint64_t& off,const uint64_t&m, std::set<uint64_t>& occ,G& );

    // return the preorder of the node definition for rule X
    template <typename G>
    uint64_t findPreorderNodeDefinition(const uint64_t& X,G&);
    template <typename G>
    uint64_t num_rules(G&);

    struct path_element{
        uint64_t preorder;
        uint32_t ch_rank;
        path_element(const uint64_t&p, const uint32_t&r):preorder(p),ch_rank(r){}
    };

    // decend in the tree searching the position p and return the path bottom-up in _path
    template <typename G>
    void path(const uint64_t & prenode,const uint64_t &p, G&,std::stack<path_element>& _path );
    template <typename G>
    std::string extraction(const uint64_t &p,const uint64_t &l, G&);

    template<typename G>
    uint64_t child(const uint64_t &node, const uint32_t &i, const G &grammar);

    template<typename G>
    uint64_t labelInternalNode(const uint64_t &prenode, const G &grammar);

    //return the preorder of the parent given the preorder of the node
    template<typename G>
    uint64_t preParent(const uint64_t &prenode, const G &grammar);

    template<typename G>
    uint64_t preorderSelect(const uint64_t &prenode, const G &grammar);
    template<typename G>
    uint64_t preorder(const uint64_t &node, const G &grammar);

    template<typename G>
    uint32_t children(const uint64_t &node, const G &grammar);

    template<typename G>
    uint32_t runLen(const uint64_t &prenode, const G &grammar);

    template<typename G>
    bool isLeaf(const uint64_t &prenode, const G &grammar);

    template<typename G>
    bool isRun(const uint64_t &prenode, const G &grammar);

    template<typename G>
    uint64_t firstOccPrenode(const uint64_t &prenode, const G &grammar);

    template<typename G>
    uint64_t firstOccLabel(const uint64_t &prenode, const G &grammar);


    template<typename F, typename G>
    bool dfs_leaf(const uint64_t &pre_node, const uint64_t &node, const F &f, G &cgrammar);

    template<typename F, typename G>
    bool dfs_leaf_run(const uint64_t &pre_node, uint64_t &node, const F &f, G &cgrammar);

    template<typename F, typename G>
    bool dfs_leaf_block(const uint64_t &pre_node, uint64_t &node, const F &f, G &cgrammar);

    template<typename F, typename G>
    bool dfs_mirror_leaf(const uint64_t &pre_node, const uint64_t &node, const F &f, G &cgrammar);

    template<typename F, typename G>
    bool dfs_mirror_leaf_run(const uint64_t &pre_node, uint64_t &node, const F &f, G &cgrammar);

    template<typename F, typename G>
    bool dfs_mirror_leaf_block(const uint64_t &pre_node, uint64_t &node, const F &f, G &cgrammar);


    template<
            typename tree_topology = trees::dfuds_tree<>,
            typename sq_x = GolynskiSeq,

            typename bv_l = sdsl::sd_vector<>,
            typename bv_run  = sdsl::sd_vector<>,
            typename v_run  = sdsl::int_vector<>,
            typename v_tree_anc   = sdsl::int_vector<>,
            typename v_tree_off_anc   = sdsl::int_vector<>,
            typename v_tree_next   = sdsl::int_vector<>,
            typename bv_z = sdsl::sd_vector<>,
            typename bv_c  = sdsl::sd_vector<>,
            typename perm_x = sdsl::inv_perm_support<INV_PERM_PI>

    >
    class lc_grammar {

    public:
        lc_grammar() = default;


        tree_topology m_tree;
        bv_c m_c;
        typename bv_c::rank_1_type m_c_rank;
        bv_l m_l;
        typename bv_l::rank_1_type m_l_rank;
        typename bv_l::select_1_type m_l_select;
        bv_z m_z;
        typename bv_z::rank_1_type m_z_rank;
        typename bv_z::select_1_type m_z_select;
        sq_x m_x;
        perm_x m_inv_pi;
        sdsl::int_vector<> m_pi;

        v_tree_anc m_ancs;
        v_tree_off_anc m_off;
        v_tree_next m_next;
        bv_run m_bv_run;
        typename bv_run::rank_1_type m_bv_run_rank;
        v_run m_v_run;

        sdsl::int_vector<> m_alph;


    };

    template<typename F, typename G>
    bool dfs_leaf(const uint64_t &pre_node, const uint64_t &node, const F &f, G &cgrammar) {

//        auto node = preorderSelect(pre_node,cgrammar);
        uint64_t X = 0;
        if (isTerminal(pre_node, X, cgrammar)) {
            bool keep = f(pre_node, node, X, cgrammar);
            return keep;
        }
        if (isLeaf(pre_node, cgrammar)) {
            //second occ.
            auto fpre_node = firstOccPrenode(pre_node, cgrammar);
            auto fnode = preorderSelect(fpre_node,cgrammar);

            return dfs_leaf(fpre_node, fnode, f, cgrammar);
        } else {
            //run-node
            if (isRun(pre_node, cgrammar)) {
                return dfs_leaf_run(pre_node, node, f, cgrammar);
            } else {
                //block-node
                return dfs_leaf_block(pre_node, node, f, cgrammar);
            }
        }

    }


    template<typename F, typename G>
    bool dfs_leaf_run(const uint64_t &pre_node, const uint64_t &node, const F &f, G &cgrammar) {
        uint32_t len = runLen(pre_node, cgrammar);
        for (uint32_t i = 0; i < len; ++i) {
            auto chnode = child(node, 1, cgrammar);
            bool keep = dfs_leaf(preorder(chnode, cgrammar), chnode, f, cgrammar);
            if (!keep) return keep;
        }
        return true;
    }

    template<typename F, typename G>
    bool dfs_leaf_block(const uint64_t &pre_node, const uint64_t &node, const F &f, G &cgrammar) {
        uint32_t n = children(node, cgrammar);
        for (uint32_t i = 0; i < n; ++i) {
            auto chnode = child(node, i + 1, cgrammar);
            bool keep = dfs_leaf(preorder(chnode, cgrammar), chnode, f, cgrammar);
            if (!keep) return keep;
        }

        return true;
    }


    template<typename F, typename G>
    bool dfs_mirror_leaf(const uint64_t &pre_node, const uint64_t &node, const F &f, G &cgrammar) {

        uint64_t X = 0;
        if (isTerminal(pre_node, X, cgrammar)) {
            bool keep = f(pre_node, node, X, cgrammar);
            return keep;
        }
        if (isLeaf(pre_node, cgrammar)) {
            //second occ.
            auto fpre_node = firstOccPrenode(pre_node, cgrammar);
            auto fnode = preorderSelect(fpre_node,cgrammar);

            return dfs_mirror_leaf(fpre_node, fnode, f, cgrammar);
        } else {
            //run-node
            if (isRun(pre_node, cgrammar)) {
                return dfs_mirror_leaf_run(pre_node, node, f, cgrammar);
            } else {
                //block-node
                return dfs_mirror_leaf_block(pre_node, node, f, cgrammar);
            }
        }

    }



    template<typename F, typename G>
    bool dfs_mirror_leaf_run(const uint64_t &pre_node, const uint64_t &node, const F &f, G &cgrammar) {
        uint32_t len = runLen(pre_node, cgrammar);
        for (uint32_t i = 0; i < len; ++i) {
            auto chnode = child(node, 1, cgrammar);
            bool keep = dfs_mirror_leaf(preorder(chnode, cgrammar), chnode, f, cgrammar);
            if (!keep) return keep;
        }

        return true;
    }

    template<typename F, typename G>
    bool dfs_mirror_leaf_block(const uint64_t &pre_node, const uint64_t &node, const F &f, G &cgrammar) {
        uint32_t n = children(node, cgrammar);
        for (uint32_t i = n; i > 0; --i) {
            auto chnode = child(node, i, cgrammar);
            bool keep = dfs_mirror_leaf(preorder(chnode, cgrammar), chnode, f, cgrammar);
            if (!keep) return keep;
        }

        return true;
    }


};







#endif //RRPAIR_COMPRESSED_GRAMMAR_HPP
