
/**
 * Copyright by Kazuya Tsuruta
 */

#ifndef RRPAIR_LYNDON_H
#define RRPAIR_LYNDON_H

#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include "any.hpp"

#include <cassert>
#include <climits>
#include <cstring>


namespace lyndon{


    typedef unsigned int uint;
    typedef unsigned char uchar;

    class LyndonSLP {
    public:

        struct SubString {
            static const uchar *TEXT;
            static uint TEXT_SIZE;

            uint start_;
            uint end_;

            inline SubString(uint start, uint end);

            inline SubString();

            inline uchar operator[](uint index) const;

            inline bool operator<(const SubString &value) const;

            inline uchar get_char(uint index) const;

            inline uint size() const;

            inline static bool is_greater(const SubString &a, const SubString &b);

            inline static bool is_greater_reversed(const SubString &a,
                                                   const SubString &b);

            inline static bool is_greater(uchar a, uchar b);

            inline SubString substring(uint start, uint end) const;

            inline SubString prefix(uint end) const;

            inline SubString suffix(uint start) const;

            inline static SubString concat(const SubString &value0,
                                           const SubString &value1);

            std::string to_string() const;

            inline static uint get_size_lcp(const SubString &value0,
                                            const SubString &value1);
        };

        class PatriciaTreeNode {
        public:
            typedef std::pair<SubString, PatriciaTreeNode *> Edge;
            std::map<uchar, Edge> children_;
            uint var_id_;

            PatriciaTreeNode();
            ~PatriciaTreeNode();

            inline bool has_var();

            inline void set_var_id(uint var_id);

            bool has(const SubString &text);

            inline uint size();

            uint get_var_size();

            inline PatriciaTreeNode *insert_child(const SubString &text);

            inline void add_child(const SubString &text, PatriciaTreeNode *node);

            void print(std::string prefix = "");
        };

        class SLPTreeNode {
        public:
            std::unordered_map<uint, SLPTreeNode *> children_;
            uint var_id_;

            SLPTreeNode(uint var_id);
            ~SLPTreeNode();

            inline bool has_var();

            inline void set_var_id(uint var_id);

            // bool has(const SubString &text);

            inline uint size();

            uint get_var_size();

            inline SLPTreeNode *get_child(uint edge);

            inline SLPTreeNode *add_child(uint edge, uint new_var_id);

            void print(std::string prefix = "");
        };
        //}; // namespace PatriciaTree

        // const std::string *SubString::TEXT = nullptr;
        class SLP {
        public:
            class Rule {
            public:
                uint left;
                uint right;
                SubString value_;
                uint id;

                Rule(uint l, uint r);
                Rule(uchar symbol, SubString value);
                Rule(uint l, uint r, SubString value);

                static bool is_greater(const Rule &rule0, const Rule &rule1);
                static bool is_greater_reversed(const Rule &rule0,
                                                const Rule &rule1);

                uint get_length() const;
            };

            // const static uint LETTER_FLAG = (uint)-1;
            const static int LETTER_FLAG = UINT_MAX;

        public:
            std::vector<Rule> rules_;
            uint alphabet_size_;

        public:
            SLP();

            inline uint size();

            inline uint last_id();

            // return id for added variable
            inline uint push(uchar symbol, SubString value);
            inline uint push(uint right, uint left, SubString value);

            inline bool is_letter(uint id);

            inline std::vector<Rule> get_rules() const;

            int val(uint id, std::string &ret_val);

            void print_val(uint id);

            void print_values();

            void print_rules();

            void print_istr(const std::vector<uint> &istr);

            void decomp(std::string &ret_str);

            void decomp(const std::vector<uint> &istr, std::string &ret_str);

            uint get_height(uint id) const;

            void tight_size() const;
        };

        class CFGrammar {
        public:
            typedef std::vector<uint> Rule;
            const static int LETTER_FLAG = UINT_MAX;

            std::vector<Rule> rules_;

            CFGrammar(SLP slp);
        };

    public:
        LyndonSLP();

        int compress(const uchar *text, uint textlength);
        std::vector<SLP::Rule> get_rules() const;
        std::vector<uint> get_compressed_text() const;
        uint get_alphabet_size() const;

    public:
        SLP grammar_;  // compressed dictionary

    private:
        std::vector<uint> compressed_text_;  // compressed text

        // SLP compress(const std::string &text);

        // void set_alphabet_order(const std::vector<int> &order);
    };


}




#endif //RRPAIR_LYNDON_H
