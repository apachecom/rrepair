
#include "../../include/lyndon/lyndon.h"


using namespace lyndon;



const uint ALPHABET_SIZE = 256;
uchar alphabet_order[ALPHABET_SIZE];

inline LyndonSLP::SubString::SubString(uint start, uint end) {
    // std::cerr << "start : " << start << ", end : " << end << std::endl;
    assert(start < TEXT_SIZE);
    assert(end < TEXT_SIZE);
    assert(start <= end);

    start_ = start;
    end_ = end;
}

inline LyndonSLP::SubString::SubString() {
    start_ = UINT_MAX;
    end_ = UINT_MAX;
}

inline uchar LyndonSLP::SubString::operator[](uint index) const {
    return get_char(index);
}

inline uchar LyndonSLP::SubString::get_char(uint index) const {
    return TEXT[start_ + index];
}

inline uint LyndonSLP::SubString::size() const { return end_ - start_ + 1; }

inline LyndonSLP::SubString LyndonSLP::SubString::substring(uint start,
                                                            uint end) const {
    assert(start < size());
    assert(end < size());
    assert(start <= end);

    return SubString(start_ + start, start_ + end);
}

inline LyndonSLP::SubString LyndonSLP::SubString::prefix(uint end) const {
    assert(end < size());
    return substring(0, end);
}

inline LyndonSLP::SubString LyndonSLP::SubString::suffix(uint start) const {
    assert(start < size());
    return substring(start, size() - 1);
}

inline LyndonSLP::SubString LyndonSLP::SubString::concat(
        const SubString &value0, const SubString &value1) {
    assert(value0.end_ + 1 == value1.start_ ||
           value0.start_ == value1.end_ + 1);

    return SubString(std::min(value0.start_, value1.start_),
                     std::max(value0.end_, value1.end_));
}

inline bool LyndonSLP::SubString::operator<(const SubString &value) const {
    return is_greater(*this, value);
}

std::string LyndonSLP::SubString::to_string() const {
    assert(start_ < TEXT_SIZE);
    assert(end_ < TEXT_SIZE);
    assert(start_ <= end_);

    return std::string((const char *)TEXT).substr(start_, size());
}

inline uint LyndonSLP::SubString::get_size_lcp(const SubString &value0,
                                               const SubString &value1) {
    uint size = std::min(value0.size(), value1.size());
    for (uint i = 0; i < size; ++i) {
        if (value0[i] != value1[i]) {
            return i;
        }
    }
    return size;
}

inline bool LyndonSLP::SubString::is_greater(const SubString &a,
                                             const SubString &b) {
    uint min_size = std::min(a.size(), b.size());

    int ret = strncmp((const char *)TEXT + a.start_,
                      (const char *)TEXT + b.start_, min_size);
    if (ret < 0) {
        return true;
    } else if (ret > 0) {
        return false;
    } else {
        return (a.size() < b.size());
    }

    // uchar a_char, b_char;
    // for (uint i = 0; i < min_size; ++i)
    // {
    //     a_char = a.get_char(i);
    //     b_char = b.get_char(i);
    //     if (a_char != b_char)
    //     {
    //         return is_greater(a_char, b_char);
    //     }
    // }

    // return (a.size() < b.size());
}

typedef int maxReg;
inline bool LyndonSLP::SubString::is_greater_reversed(const SubString &a,
                                                      const SubString &b) {
    const static uint STEP = sizeof(maxReg) / sizeof(char);

    uint min_size = std::min(a.size(), b.size());
    uint i = 0;
    const maxReg *a_reg = (maxReg *)(TEXT + a.end_ + 1 - STEP);
    const maxReg *b_reg = (maxReg *)(TEXT + b.end_ + 1 - STEP);
    for (; i + STEP <= min_size; i += STEP) {
        if (*a_reg != *b_reg) {
            break;
        };
        --a_reg;
        --b_reg;
    }

    uchar a_char, b_char;
    for (; i < min_size; ++i) {
        a_char = TEXT[a.end_ - i];
        b_char = TEXT[b.end_ - i];
        if (a_char != b_char) {
            return is_greater(a_char, b_char);
        }
    }

    return (a.size() < b.size());
}

inline bool LyndonSLP::SubString::is_greater(uchar a, uchar b) {
    return (char)a < (char)b;
    // return alphabet_order[a] < alphabet_order[b];
}

LyndonSLP::PatriciaTreeNode::PatriciaTreeNode() { var_id_ = UINT_MAX; }

LyndonSLP::PatriciaTreeNode::~PatriciaTreeNode() {
    for (std::map<uchar, Edge>::iterator it = children_.begin();
         it != children_.end(); ++it) {
        delete it->second.second;
    }
}

inline bool LyndonSLP::PatriciaTreeNode::has_var() {
    return var_id_ != UINT_MAX;
}

inline void LyndonSLP::PatriciaTreeNode::set_var_id(uint var_id) {
    var_id_ = var_id;
}

bool LyndonSLP::PatriciaTreeNode::has(const SubString &text) {
    if (children_.count(text[0]) == 0) {
        return false;
    } else {
        Edge edge = children_[text[0]];
        SubString label = edge.first;
        PatriciaTreeNode *child = edge.second;
        uint size_lcp = SubString::get_size_lcp(text, label);
        if (size_lcp == label.size()) {
            if (size_lcp == text.size()) {
                return child->has_var();
            } else {
                // case of size_lcp == label.size() && size_lcp < textlength
                return child->has(text.suffix(size_lcp));
            }
        } else {
            // case of size_lcp < label.size()
            return false;
        }
    }
}

inline uint LyndonSLP::PatriciaTreeNode::size() {
    uint total_size = 0;
    for (std::map<uchar, Edge>::iterator it = children_.begin();
         it != children_.end(); ++it) {
        total_size += it->second.second->size();
    }
    ++total_size;
    return total_size;
}

uint LyndonSLP::PatriciaTreeNode::get_var_size() {
    uint total_size = 0;
    if (has_var()) {
        ++total_size;
    }
    for (std::map<uchar, Edge>::iterator it = children_.begin();
         it != children_.end(); ++it) {
        total_size += it->second.second->get_var_size();
    }
    return total_size;
}

inline LyndonSLP::PatriciaTreeNode *LyndonSLP::PatriciaTreeNode::insert_child(
        const SubString &text) {
    if (children_.count(text[0]) == 0) {
        PatriciaTreeNode *new_node = new PatriciaTreeNode();
        add_child(text, new_node);
        return new_node;
    } else {
        Edge edge = children_[text[0]];
        SubString label = edge.first;
        PatriciaTreeNode *child = edge.second;
        uint size_lcp = SubString::get_size_lcp(text, label);
        assert(0 < size_lcp);
        if (size_lcp == label.size()) {
            if (size_lcp == text.size()) {
                return child;
            } else {
                // case of size_lcp == label.size() && size_lcp < text.size()
                return child->insert_child(text.suffix(size_lcp));
            }
        } else {
            // case of size_lcp < label.size()
            PatriciaTreeNode *internal_node = new PatriciaTreeNode();
            add_child(label.prefix(size_lcp - 1), internal_node);
            internal_node->add_child(label.suffix(size_lcp), child);
            if (size_lcp == text.size()) {
                return internal_node;
            } else {
                PatriciaTreeNode *new_node = new PatriciaTreeNode();
                internal_node->add_child(text.suffix(size_lcp), new_node);
                return new_node;
            }
        }
    }
}

inline void LyndonSLP::PatriciaTreeNode::add_child(const SubString &text,
                                                   PatriciaTreeNode *node) {
    children_[text[0]] = Edge(text, node);
}

// void LyndonSLP::PatriciaTreeNode::print(std::string prefix)
// {
//     if (has_var())
//     {
//         std::cerr << "Lyndon word[" << var_id_ << "] : " << prefix <<
//         std::endl;
//     }
//     else
//     {
//         //std::cerr << "- " << prefix << std::endl;
//     }

//     for (std::map<char, Edge>::iterator it = children_.begin(); it !=
//     children_.end(); ++it)
//     {
//         it->second.second->print(prefix + it->second.first.to_string());
//     }
// }

LyndonSLP::SLPTreeNode::SLPTreeNode(uint var_id) { var_id_ = var_id; }

LyndonSLP::SLPTreeNode::~SLPTreeNode() {
    for (std::unordered_map<uint, SLPTreeNode *>::iterator it =
            children_.begin();
         it != children_.end(); ++it) {
        delete it->second;
    }
}

inline bool LyndonSLP::SLPTreeNode::has_var() { return var_id_ != UINT_MAX; }

inline void LyndonSLP::SLPTreeNode::set_var_id(uint var_id) {
    var_id_ = var_id;
}

inline uint LyndonSLP::SLPTreeNode::size() {
    uint total_size = 0;
    for (std::unordered_map<uint, SLPTreeNode *>::iterator it =
            children_.begin();
         it != children_.end(); ++it) {
        total_size += it->second->size();
    }
    ++total_size;
    return total_size;
}

uint LyndonSLP::SLPTreeNode::get_var_size() {
    uint total_size = 0;
    if (has_var()) {
        ++total_size;
    }
    for (std::unordered_map<uint, SLPTreeNode *>::iterator it =
            children_.begin();
         it != children_.end(); ++it) {
        total_size += it->second->get_var_size();
    }
    return total_size;
}

inline LyndonSLP::SLPTreeNode *LyndonSLP::SLPTreeNode::get_child(uint edge) {
    if (children_.count(edge) == 0) {
        return nullptr;
    } else {
        return children_[edge];
    }
}

inline LyndonSLP::SLPTreeNode *LyndonSLP::SLPTreeNode::add_child(
        uint edge, uint new_var_id) {
    assert(children_.count(edge) == 0);
    SLPTreeNode *new_node = new SLPTreeNode(new_var_id);
    children_[edge] = new_node;
    return new_node;
}

// void LyndonSLP::PatriciaTreeNode::print(std::string prefix)
// {
//     if (has_var())
//     {
//         std::cerr << "Lyndon word[" << var_id_ << "] : " << prefix <<
//         std::endl;
//     }
//     else
//     {
//         //std::cerr << "- " << prefix << std::endl;
//     }

//     for (std::map<char, Edge>::iterator it = children_.begin(); it !=
//     children_.end(); ++it)
//     {
//         it->second.second->print(prefix + it->second.first.to_string());
//     }
// }

LyndonSLP::SLP::Rule::Rule(uint l, uint r) {
    left = l;
    right = r;
}

LyndonSLP::SLP::Rule::Rule(uchar symbol, LyndonSLP::SubString value) {
    left = symbol;
    right = LETTER_FLAG;
    value_ = value;
}

LyndonSLP::SLP::Rule::Rule(uint l, uint r, LyndonSLP::SubString value) {
    left = l;
    right = r;
    value_ = value;
}

bool LyndonSLP::SLP::Rule::is_greater(const LyndonSLP::SLP::Rule &rule0,
                                      const LyndonSLP::SLP::Rule &rule1) {
    return SubString::is_greater(rule0.value_, rule1.value_);
}

bool LyndonSLP::SLP::Rule::is_greater_reversed(
        const LyndonSLP::SLP::Rule &rule0, const LyndonSLP::SLP::Rule &rule1) {
    return SubString::is_greater_reversed(rule0.value_, rule1.value_);
}

uint LyndonSLP::SLP::Rule::get_length() const { return value_.size(); }

LyndonSLP::SLP::SLP() {
    push(0, 0, SubString());
    alphabet_size_ = 0;
}

inline uint LyndonSLP::SLP::size() { return rules_.size(); }

inline uint LyndonSLP::SLP::last_id() { return rules_.size() - 1; }

// return id for added variable
inline uint LyndonSLP::SLP::push(uchar symbol, SubString value) {
    ++alphabet_size_;
    return push(symbol, LETTER_FLAG, value);
}

// return id for added variable
inline uint LyndonSLP::SLP::push(uint right, uint left, SubString value) {
    rules_.push_back(Rule(right, left, value));
    rules_.back().id = last_id();
    return this->last_id();
}

inline bool LyndonSLP::SLP::is_letter(uint id) {
    return (rules_[id].right == LETTER_FLAG);
}

inline std::vector<LyndonSLP::SLP::Rule> LyndonSLP::SLP::get_rules() const {
    return rules_;
}

int LyndonSLP::SLP::val(uint id, std::string &ret_val) {
    // if (is_letter(id))
    // {
    //     ret_val = (char)(id);
    //     return 1;
    // }
    // else if (id > this->last_id())
    // {
    //     return 0;
    // }
    // std::string temp_val;
    // val(this->l_id(id), temp_val);
    // ret_val = temp_val;
    // val(this->r_id(id), temp_val);
    // ret_val += temp_val;

    return 1;
}

void LyndonSLP::SLP::print_val(uint id) {
    std::string val;
    if (this->val(id, val)) {
        std::cout << val;
    } else {
        std::cerr << "error (out of range for id [" << id << "]";
    }
}

void LyndonSLP::SLP::print_values() {
    for (size_t i = 1; i < rules_.size(); i++) {
        std::string value = rules_[i].value_.to_string();
        if (value[0] == 1) {
            value[0] = '$';
        }
        LOG("grammar[" << i << "] : " << rules_[i].id << " : " << value);
    }
}

void LyndonSLP::SLP::print_rules() {
    LOG(rules_[0].id << " -> " << rules_[0].left << " " << rules_[0].right
                     << " : " << rules_[0].get_length() << " : ");
    for (size_t i = 1; i < rules_.size(); i++) {
        std::string value = rules_[i].value_.to_string();
        if (value[0] == 1) {
            value[0] = '$';
        }
        LOG(rules_[i].id << " -> " << rules_[i].left << " " << rules_[i].right
                         << " : " << rules_[i].get_length() << " : " << value);
    }
}

uint LyndonSLP::SLP::get_height(uint id) const {
    if (rules_[id].right == LETTER_FLAG) {
        return 1;
    }

    return std::max(get_height(rules_[id].left), get_height(rules_[id].right)) +
           1;
}

void LyndonSLP::SLP::print_istr(const std::vector<uint> &istr) {
#ifdef DEBUG
    std::cout << "size " << istr.size() << std::endl;
#endif
    for (uint i = 0; i < istr.size(); ++i) {
        if (this->is_letter(istr[i])) {
            std::cout << (uchar)(istr[i]);
        } else {
            std::cout << "[" << istr[i] << "]";
        }
    }
    std::cout << std::endl;
}

void LyndonSLP::SLP::decomp(std::string &ret_str) {
    val(this->last_id(), ret_str);
}

void LyndonSLP::SLP::decomp(const std::vector<uint> &istr,
                            std::string &ret_str) {
    ret_str = "";
    for (uint i = 0; i < istr.size(); ++i) {
        std::string temp_str;
        val(istr[i], temp_str);
        ret_str += temp_str;
    }
}

void LyndonSLP::SLP::tight_size() const {
    // ルールの右辺での出現回数を計算
    std::vector<uint> rules_occurrence(rules_.size(), 0);
    for (size_t i = 0; i < rules_.size(); i++) {
        if (rules_[i].right == LETTER_FLAG) continue;
        ++rules_occurrence[rules_[i].left];
        ++rules_occurrence[rules_[i].right];
    }

    // ルールの右辺での出現回数が1の変数の個数を計算
    uint count = 0;
    for (size_t i = 0; i < rules_.size(); i++) {
        if (rules_[i].right == LETTER_FLAG) continue;
        if (rules_occurrence[i] == 0) {
            LOG("rules_occurrence[" << i << "] : " << rules_occurrence[i]);
        }
        assert(1 <= rules_occurrence[i] || i == rules_.size() - 1);
        if (rules_occurrence[i] == 1) {
            ++count;
        }
    }
    LOG("# variables occuring just once: " << count);
    //LOG("tight_size : " << rules_.size() * 2 - count);
    // return;

    // // SLPからGrammer Compressionに変換
    // std::vector<std::vector<uint> > rules_gc(rules_.size());
    // for (size_t i = 0; i < rules_.size(); i++) {
    //     if (rules_[i].right == LETTER_FLAG) {
    //         rules_gc[i].push_back(rules_[i].left);
    //         rules_gc[i].push_back(rules_[i].right);
    //     } else {
    //         for (size_t lr = 0; lr < 2; ++lr) {
    //             uint lr_id = (lr == 0) ? rules_[i].left : rules_[i].right;
    //             while (rules_occurrence[lr_id]) {

    //             }
    //             for (size_t j = 0; j < rules_gc[rules_[i].left].size(); ++j)
    //             {
    //                 rules_gc[i].push_back(rules_gc[rules_[i].left][j]);
    //             }
    //             // LOG("rules_gc[i].size() : " << rules_gc[i].size());
    //         }
    //     }
    // TRACE("test");

    // ルールの右辺の変数連の合計を計算
    // count = 0;
    // for (size_t i = 0; i < rules_gc.size(); i++) {
    //     TRACE(i);
    //     TRACE(rules_occurrence.size());
    //     if (rules_occurrence[i] < 2) {
    //         continue;
    //     }

    //     TRACE(rules_gc[i].size());
    //     for (size_t j = 0; j < rules_gc[i].size() - 1; ++j) {
    //         TRACE(rules_gc.size());
    //         TRACE(j);
    //         if (rules_gc[i][j] == rules_gc[i][j + 1]) {
    //             ++count;
    //         }
    //     }
    //     TRACE(i);
    // }
    // LOG("num of run : " << count);
}

// const std::string *SubString::TEXT = nullptr;
const uchar *LyndonSLP::SubString::TEXT = nullptr;
uint LyndonSLP::SubString::TEXT_SIZE = 0;

LyndonSLP::LyndonSLP() {}

int LyndonSLP::compress(const uchar *text, uint textlength) {
    SubString::TEXT = text;
    SubString::TEXT_SIZE = textlength;

    std::stack<SubString> intervals;
    std::stack<SLPTreeNode *> nodes;
    SLPTreeNode *node_0;
    SLPTreeNode *node_1;
    // SLPTreeNode slp_trie;
    std::unordered_map<uchar, SLPTreeNode *> map_char_var;

    for (uint i = textlength - 1;; --i) {
        // LOG(text[i]);
        // LOG("position : " << i << ", number of slp : " << slp.size() <<
        // ", trees.size : " << trees.size());
        if (i == 0) {
            LOG("size of LF : " << intervals.size());
            LOG("Number of rules       : " << grammar_.size());
            LOG("Final sequence length : " << intervals.size());
        }
        SubString interval = SubString(i, i);
        // TODO initialize to remove under
        if (map_char_var.count(text[i]) == 0) {
            uint id = grammar_.push(text[i], interval);
            node_1 = new SLPTreeNode(id);
            map_char_var[text[i]] = node_1;
        } else {
            node_1 = map_char_var[text[i]];
        }

        while (!intervals.empty() && interval < intervals.top()) {
            // LOG("top 0   : " << top_node->var_id_);
            // LOG("top 1   : " << nodes.top()->var_id_);
            assert(node_1->has_var());
            assert(nodes.top()->has_var());

            interval = SubString::concat(interval, intervals.top());
            node_0 = node_1->get_child(nodes.top()->var_id_);
            if (node_0 == nullptr) {
                uint new_id = grammar_.push(node_1->var_id_,
                                            nodes.top()->var_id_, interval);
                node_0 = node_1->add_child(nodes.top()->var_id_, new_id);
            }
            // else
            // {
            //     //grammar_.rules_[new_top_node->var_id_].occ = i;
            // }

            node_1 = node_0;

            intervals.pop();
            nodes.pop();
        }
        intervals.push(interval);
        nodes.push(node_1);

        if (i == 0) {
            break;
        }
    }

    // LOG("hgoe" << slp.size());
    // slp.print_val(99);
    // slp.print_val(265);
    // slp.print_val(265);
    // slp.print_rules();
    while (!nodes.empty()) {
        // LOG("nodes.top()->var_id_ = " << nodes.top()->var_id_);
        compressed_text_.push_back(nodes.top()->var_id_);
        nodes.pop();
    }

#ifdef DEBUG
    // slp.print_rules();
    // lyndon_trie.print();
#endif

    // grammar_.print_rules();

    // LOG("number of run : " << count);

    // const auto endTime = std::chrono::system_clock::now();
    // const auto timeSpan = endTime - startTime;
    // std::cerr << "time: " <<
    // std::chrono::duration_cast<std::chrono::milliseconds>(timeSpan).count()
    // <<
    // "[ms]" << std::endl;

    LOG("intervals.size : " << intervals.size());
    LOG("SLP size : " << grammar_.size());
    LOG("alphabet_size : " << get_alphabet_size());

    LOG("# rules X->a: " << get_alphabet_size());
    LOG("# rules X->YZ: " << grammar_.size() - get_alphabet_size());


    // for (size_t i = 0; i < compressed_text_.size(); i++)
    // {
    //     LOG("height of ctext : " << compressed_text_[i] << " : " <<
    //     grammar_.get_height(compressed_text_[i]));
    // }

    // std::cout << "number of lyndon word : " << lyndon_trie.get_var_size()
    // << std::endl; std::cout << "lyndon_trie.size : " <<
    // lyndon_trie.size() << std::endl;
    grammar_.tight_size();

    return 0;
    assert(false);
}

std::vector<LyndonSLP::SLP::Rule> LyndonSLP::get_rules() const {
    return grammar_.get_rules();
}

std::vector<uint> LyndonSLP::get_compressed_text() const {
    return compressed_text_;
}

uint LyndonSLP::get_alphabet_size() const { return grammar_.alphabet_size_; }
