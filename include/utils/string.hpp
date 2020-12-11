//
// Created by via56 on 11-12-20.
//

#ifndef RRPAIR_STRING_HPP
#define RRPAIR_STRING_HPP

#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <random>
#include <fstream>

namespace utilString {

    typedef std::vector<char> char_array;

    char_array charset();
    std::string random_string( size_t length, std::function<char(void)> rand_char );
    std::string generate_random_string( size_t length );
}



#endif //RRPAIR_STRING_HPP
