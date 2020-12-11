////
//// Created by via56 on 06-12-20.
////
//
//
//#include "../../include/indices/bs_index.hpp"
//
//using namespace lc;
//using namespace indices;
//
//
//template <>
//void indices::findPartition<lc::g_bs_index<>>(const char* ,const uint64_t & len,std::set<uint32_t>&, lc::g_bs_index<>* indice);
//
//template <>
//bool indices::findGridRanges<lc::g_bs_index<>>(const char* ,const uint64_t & len,const uint64_t & q,
//                                     grid_range &, lc::g_bs_index<>& indice);
//
//template <>
//void lc::gridSearch<g_bs_index>(const grid_range& range, const uint64_t & q,std::vector<primaryOcc>& occ, g_bs_index* indice);
//
//template <>
//void lc::findSecondaryOcc<g_bs_index>(const primaryOcc&, const uint64_t &, std::set<uint64_t> &pos,g_bs_index*);
//
//
//
//template <>
//void lcg::findPartition<g_bs_index>(const char* ,const uint64_t & len,std::set<uint32_t>&, g_bs_index& indice);
//
//template <>
//bool lcg::findGridRanges<g_bs_index>(const char* ,const uint64_t & len,const uint64_t & q,
//                                     grid_range &, g_bs_index& indice);
//
//template <>
//void lcg::gridSearch<g_bs_index>(const grid_range& range, const uint64_t & q,std::vector<primaryOcc>& occ, g_bs_index* indice);
//
//template <>
//void lcg::findSecondaryOcc<g_bs_index>(const primaryOcc&, const uint64_t &, std::set<uint64_t> &pos,g_bs_index*);
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//template <>
//void lcg::findPartition<g_bs_index>(const char* pattern, const uint64_t & len, std::set<uint32_t>& positions, g_bs_index& indice){
//    parse_pattern(pattern,indice.fingerprints,indice.pi,positions);
//}
//
//
//template <>
//bool lc::findGridRanges<g_bs_index>(const char* pattern ,const uint64_t & len,const uint64_t & q,
//                                     grid_range& range, g_bs_index& indice){
//
//    auto cmp_rev_prefix_rule  = [&indice,&q,&pattern](const uint64_t & rule_id){
//        // compute node definiton preorder of the rule
//        uint64_t prenode = lcg::findPreorderNodeDefinition(rule_id,indice._g);
//        int r =  lcg::cmp_prefix_rule(prenode,pattern,q - 1,indice._g);
//        return r;
//
//    };
//    uint64_t row_1 = 1,row_2 = lcg::num_rules(indice._g);
//    //search lower
//    if(!lcg::lower_bound(row_1,row_2,cmp_rev_prefix_rule))  return false;
//    //search upper
//    row_2 = lcg::num_rules(indice._g);
//    if(!lcg::upper_bound(row_1,row_2,cmp_rev_prefix_rule))  return false;
//
//
//    auto cmp_suffix_grammar_rule  = [&indice,&q,&pattern](const uint64_t & suffix_id){
//        // compute node definiton preorder of the rule
//        uint64_t prenode = indice._grid.first_label_col(suffix_id);
//        int r =  lcg::cmp_suffix_grammar(prenode,pattern,q,indice._g);
//        return r;
//
//    };
//
//    uint64_t col_1 = 1,col_2 = indice._grid.n_columns();
//    //search lower
//    if(!lcg::lower_bound(col_1,col_2,cmp_suffix_grammar_rule))  return false;
//    //search upper
//    col_2 = indice._grid.n_columns();
//    if(!lcg::upper_bound(col_1,col_2,cmp_suffix_grammar_rule))  return false;
//
//    // return the range and true
//    range = grid_range(row_1,row_2,col_1,col_2);
//    return true;
//}
//
//
//template <>
//void lc::gridSearch<g_bs_index>(const grid_range& range, const uint64_t & pattern_off,std::vector<primaryOcc>& occ, g_bs_index* indice) {
//
//    std::vector<uint64_t> cols;
//    indice->_grid.range(range, cols);
//    occ.resize(cols.size());
//
//    for (uint32_t i = 0; i < cols.size();i++) {
//        uint64_t prenode = indice->_grid.first_label_col(cols[i]);
//        uint64_t off = lcg::off(prenode,indice->_g) + pattern_off;
//        uint64_t preancs = lcg::ancs(prenode,indice->_g);
//        occ[i] = primaryOcc(preancs,off);
//    }
//}
//
//template <>
//void lc::findSecondaryOcc<g_bs_index>(const primaryOcc& occ,const uint64_t & pattern_len,std::set<uint64_t> &pos,g_bs_index* indice){
//    lcg::findSecondOccPreorderNode(occ.preorder_node,occ.off,pattern_len,pos,indice->_g);
//}
