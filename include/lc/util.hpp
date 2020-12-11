//
// Created by via56 on 11-11-20.
//

#ifndef LCP_LCG_UTIL_HPP
#define LCP_LCG_UTIL_HPP


#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include <unordered_map>
#include <sdsl/bit_vectors.hpp>


namespace lc {



    typedef std::unordered_map<std::string, uint64_t, std::hash<std::string>> HT_1_DUMB;
    typedef std::unordered_map<uint64_t, uint64_t> HT_2_DUMB;

    void permutation( const uint64_t& len, std::vector<uint64_t> &pi);

    template<typename HT1, typename HT2>
    bool fruns(const std::string& pattern,
               std::vector<uint64_t>& MT,
               std::vector<uint64_t>& P,
               HT1& H,
               HT2& Pi,
               HT2& new_Pi,
               uint64_t& num_rules,
               std::vector<uint64_t>& RUNS,
               std::vector<uint64_t>& PRUNS
    );

    template<typename HT1, typename HT2>
    bool fblock(
            const std::string& pattern,
            std::vector<uint64_t>& MT,
            std::vector<uint64_t>& P,
            HT1& H,
            HT2& Pi,
            uint64_t& num_rules,
            std::vector<uint64_t>& BLOCKS,
            std::vector<uint64_t>& PBLOCKS
    );

    template<typename HT1, typename HT2>
    void parse_pattern(const std::string &s,  HT1 &H,  HT2 &pi, std::set<uint32_t> &P);

    /**
     * search the rule in the hash table and return the id of the rule
     * if the rule does not exist return 0
     * */
    template<typename hash_type, typename HT1>
    hash_type lookfor_rule(const std::string &expansion, HT1 &H);



    //todo implement a exponencial search
    template <typename K>
    bool exponecial_search(uint64_t &len,uint64_t  &p) {
        return false;
    }

    template <typename K>
    bool binary_search(uint64_t &l,uint64_t  &r,const K &f){

        int c = f(l);
        if(c < 0) return false;
        if(c == 0) {
            r = l;
            return true;
        }
        c = f(r);
        if(c > 0) return false;
        if(c == 0) {
            l = r;
            return true;
        }

        while(l <= r){
            uint64_t mid = (l+r)/2;
            int c = f(mid);
            if(c < 0){
                r = mid -1;
            }else{
                if(c > 0)
                    l = mid+1;
                else{
                    return true;
                }
            }
        }

        return false;
    }


    template<typename K>
    bool lower_bound(bool &found , uint64_t &lr,uint64_t  &hr, const K &f)  {
        found = false;
        while(lr < hr){
            uint64_t mid = (lr+hr)/2;
            int c = f(mid);
            if(c < 0){
                hr = mid -1;
            }else{
                if(c > 0)
                    lr = mid+1;
                else{
                    hr = mid;
                    found = true;
                }
            }
        }
        if(!found && lr == hr && f(lr) == 0 ) {
            found = true;
            return true;
        }
        return false;
    }

    template<typename K>
    bool upper_bound(bool &found, uint64_t  &lr,uint64_t  &hr, const K &f)  {
        found = false;
        while(lr < hr){
            uint64_t mid = ceil((lr+hr)/2.0);
            int c = f(mid);
            if(c < 0){
                hr = mid -1;
            }else{
                if(c > 0)
                    lr = mid+1;
                else{
                    lr = mid;
                    found = true;
                }
            }
        }
        if(!found && lr == hr && f(lr) == 0 ) {
            found = true;
            return true;
        }

        return false;

    }

    template<typename K>
    bool lower_bound( uint64_t &lr,uint64_t  &hr, const K &f)  {
        bool found = false;
        while(lr < hr){
            uint64_t mid = (lr+hr)/2;
            int c = f(mid);
            if(c < 0){
                hr = mid -1;
            }else{
                if(c > 0)
                    lr = mid+1;
                else{
                    hr = mid;
                    found = true;
                }
            }
        }
        if(!found && lr == hr && f(lr) == 0 ) {
            return true;
        }
        return false;
    }

    template<typename K>
    bool upper_bound(uint64_t  &lr,uint64_t  &hr, const K &f)  {
        bool found = false;
        while(lr < hr){
            uint64_t mid = ceil((lr+hr)/2.0);
            int c = f(mid);
            if(c < 0){
                hr = mid -1;
            }else{
                if(c > 0)
                    lr = mid+1;
                else{
                    lr = mid;
                    found = true;
                }
            }
        }
        if(!found && lr == hr && f(lr) == 0 ) {
            return true;
        }

        return false;

    }


    template<typename HT1, typename HT2>
    bool lc::fruns(
            const std::string& pattern,
            std::vector<uint64_t>& MT,
            std::vector<uint64_t>& P,
            HT1& H,
            HT2& Pi,
            HT2& new_Pi,
            uint64_t& num_rules,
            std::vector<uint64_t>& RUNS,
            std::vector<uint64_t>& PRUNS){

        uint32_t off_rule = 0;
        size_t len = MT.size();
        uint64_t total_runs = 1;
        sdsl::bit_vector pos(len,0);

        for(size_t i = 1 ; i < len; i++){
            if(MT[i] == MT[i-1]){
                MT[i] = 0;
            }else{
                ++total_runs;
                pos[i-1] = 1;
            }
        }
        uint64_t c_runs = 0, last_post = 0;
        RUNS.resize(total_runs,0);
        PRUNS.resize(total_runs,0);


        for(size_t i = 0 ; i < len; i++){
            if(pos[i] == 1) {
                /**
                 * search for the run in the hash table
                 * */
                std::string exp;
                exp.resize(i-last_post + 1);
                std::copy(pattern.begin() + P[last_post],pattern.begin() + P[i] + 1,exp.begin());
                uint64_t rule = lookfor_rule<uint64_t,HT1>(exp,H);

                if(c_runs > 4 && c_runs < total_runs - 2 && rule == 0){
                    /**
                     * check if the rule exist in the hash_table
                     * if not exist stop the search the pattern is not in the Text
                     * */
                    return false;
                }

                if(rule){
                    /**
                     * rule found
                     * */
                    RUNS[c_runs] = rule;
                    new_Pi[rule] = Pi[rule];
                }else{
                    /**
                     * rule not found, but is one of the 6 extreme pos
                     * */
                    RUNS[c_runs] = num_rules + (++off_rule);
                    new_Pi[rule] = new_Pi.size() + 1;
                }
                PRUNS[c_runs] = P[i];

                ++c_runs;
                last_post = i+1;
            }
        }

        return true;

    }


    template<typename HT1, typename HT2>
    bool lc::fblock(
            const std::string& pattern,
            std::vector<uint64_t>& MT,
            std::vector<uint64_t>& P,
            HT1& H,
            HT2& Pi,
            uint64_t& num_rules,
            std::vector<uint64_t>& BLOCKS,
            std::vector<uint64_t>& PBLOCKS){

        uint32_t off_rule = 0;
        size_t len = MT.size();
        uint64_t total_block = 1;
        sdsl::bit_vector pos(len,0);

        for(size_t i = 1 ; i < len-1; i++){
            if(Pi[MT[i-1]] > Pi[MT[i]] && Pi[MT[i]] < Pi[MT[i+1]]){
                ++total_block;
                pos[i] = 1;
            }
        }
        // add the last block
        ++total_block;
        pos[len-1] = 1;
        //-----------------

        uint64_t c_blocks = 0, last_post = 0;
        BLOCKS.resize(total_block,0);
        PBLOCKS.resize(total_block,0);

        for(size_t i = 0 ; i < len; i++){
            if(pos[i] == 1) {
                /**
                 * search for the block in the hash table
                 * */
                std::string exp;
                exp.resize(i-last_post + 1);
                std::copy(pattern.begin() + P[last_post],pattern.begin() + P[i]+1,exp.begin());
                uint64_t rule = lookfor_rule<uint64_t,HT1>(exp,H);

                if(c_blocks > 4 && c_blocks < total_block - 2 && rule == 0){
                    /**
                     * check if the rule exist in the hash_table
                     * if not exist stop the search the pattern is not in the Text
                     * */
                    return false;
                }
                if(rule){
                    /**
                     * rule found
                     * */
                    BLOCKS[c_blocks] = rule;
                }else{
                    /**
                     * rule not found, but is one of the 6 extreme pos
                     * */
                    BLOCKS[c_blocks] = num_rules + (++off_rule);
                }
                PBLOCKS[c_blocks] = P[i];

                ++c_blocks;
                last_post = i+1;
            }
        }
        return true;
    }



    template<typename HT1, typename HT2>
    void lc::parse_pattern(const std::string& s,  HT1& H, HT2& pi, std::set<uint32_t>& P)
    {
        HT2 _pi;
        //inicializar el meta-texto
        uint64_t p_len = s.size();
        std::vector<uint64_t> MT(p_len,0);
        std::vector<uint64_t> MTP(p_len,0);
        for(size_t i = 0; i < p_len; ++i){
            MT[i] = (uint64_t)s[i];
            MTP[i] = i;
        }

        P.insert(0);
        P.insert(p_len-1);

        //inicializar el meta-texto
        uint64_t num_rules = H.size();

        while(true){

            std::vector<uint64_t>RUNS,PRUNS;
            fruns(s,MT,MTP,H,pi,_pi,num_rules,RUNS,PRUNS);
            P.insert(PRUNS[0]);
            if(RUNS.size() < 3) break;

            MT.clear();
            MTP.clear();
            fblock(s,RUNS,PRUNS,H,_pi,num_rules,MT,MTP);
            P.insert(MT[0]);
            P.insert(MTP[MTP.size()-1]);
            if(MT.size() <= 1) break;
        }
    }

    template<typename hash_type, typename HT1>
    hash_type lc::lookfor_rule(const std::string& expansion, HT1& H){
        auto it = H.find(expansion);
        if(it == H.end())   return 0;
        return (hash_type)it->second;
    }



    typedef uint32_t uint_t;

    /**
     * Read Dicctionary File and prepare the file to apppy Re-Pair
     * Dicctionary File Format s_1,0,s_2,0,.....,s_n,0
     *
     * Output s_1,|A|+1,s_2,|A|+2,.....,s_n,|A|+ n
     * where |A| is the max value in the alph dictionary
     ***/
    uint_t prepareDiccFileForRP(const std::string &dFile, const std::string &dout, int bytes, uint_t max_value = 0);

    /*
     * Take a sequence S of variables and create a dummy binary grammar
     * for S
     * symbols is the start id to name the new rules
     * Write in output_file
     * */
    void create_binary_grammars(const std::vector<int>& R,std::ofstream& fr_out, uint_t & symbols);


    uint_t decompress(const std::string&);







};

#endif //LCP_LCG_UTIL_HPP
