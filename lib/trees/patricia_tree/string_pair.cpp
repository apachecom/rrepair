//
// Created by via56 on 23-09-20.
//

#include <cassert>
#include <algorithm>
#include "../../../include/trees/patricia_tree/string_pair.h"

util::string_pairs::string_pairs() {
    first = 0;
    second = 0;
    text = nullptr;
    id = 0;
    c = 0;
}
util::string_pairs::string_pairs(const std::string &_text, const util::len_size &_id) :text(&_text),id(_id){
    first = 0;
    second = 0;
    c = 0;
}

util::string_pairs::string_pairs(const util::string_pairs &P):std::pair<text_size ,text_size>(P) {
    text = P.text;
    id = P.id;
    c = P.c;
    first = P.first;
    second = P.second;
}

util::string_pairs &util::string_pairs::operator=(const util::string_pairs & P) {
    text = P.text;
    id = P.id;
    c = P.c;
    first = P.first;
    second = P.second;

    return *this;
}

util::len_size util::string_pairs::get_id() const { return id;}
void util::string_pairs::set_left(const util::text_size &t) {first = t;}
void util::string_pairs::set_right(const util::text_size &t) {second = t;}
util::text_size util::string_pairs::get_left()const { return first;}
util::text_size util::string_pairs::get_right()const { return second;}
void util::string_pairs::operator++() {c++;}
util::text_size util::string_pairs::size() const { return (second-first+1);}
bool util::string_pairs::empty() { return second < first;}

ulong util::string_pairs::to_ulong() const { return (ulong)id;}

char util::string_pairs_prefix::operator[](const util::text_size &i) const {
    assert(i>=0 && i <= size());
    if(i == size())
        return 0;
    return (*text)[get_left() + i];
}

std::string util::string_pairs_prefix::toString() const {
    std::string s;
    auto first =  get_left();
    auto second =  get_right();
    s.resize(size());
    std::copy(text->begin()+first,text->begin()+second+1,s.begin());
    return s;
}

char util::string_pairs_suffix::operator[](const util::text_size &i) const {
    assert(i>=0 && i <= size());
    if(i == size())
        return 0;
    return (*text)[get_right() - i];
}

std::string util::string_pairs_suffix::toString() const {
    std::string s;
    s.resize(size());
    std::copy(text->begin()+get_left(),text->begin()+get_right()+1,s.begin());
    std::reverse(s.begin(),s.end());
    return s;
}
