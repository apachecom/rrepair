//
// Created by via56 on 11-12-20.
//

#include "../../include/utils/string.hpp"

using namespace utilString;


char_array utilString::charset()
{
    //Change this to suit
    return char_array(
            {
//                        '1','2','3','4',
//                     '5','6','7','8','9',
                    'A','B'
                    ,'C','D','E','F'
                    ,'G','H','I','J','K'
//                     ,'L','M','N','O','P'
//                     ,'Q','R','S','T','U'
//                     'V','W','X','Y','Z',
//                     'a','b','c','d','e','f',
//                     'g','h','i','j','k',
//                     'l','m','n','o','p',
//                     'q','r','s','t','u',
//                     'v','w','x','y','z'
            });
};

// given a function that generates a random character,
// return a string of the requested length
std::string utilString::random_string( size_t length, std::function<char(void)> rand_char )
{
    std::string str(length,0);
    std::generate_n( str.begin(), length, rand_char );
    return str;
}


std::string utilString::generate_random_string( size_t length ){

    //0) create the character set.
    //   yes, you can use an array here,
    //   but a function is cleaner and more flexible
    const auto ch_set = utilString::charset();

    //1) create a non-deterministic random number generator
    std::default_random_engine rng(std::random_device{}());

    //2) create a random number "shaper" that will give
    //   us uniformly distributed indices into the character set
    std::uniform_int_distribution<> dist(0, ch_set.size()-1);

    //3) create a function that ties them together, to get:
    //   a non-deterministic uniform distribution from the
    //   character set of your choice.
    auto randchar = [ ch_set,&dist,&rng ](){return ch_set[ dist(rng) ];};

    //4) set the length of the string you want and profit!
    return utilString::random_string(length,randchar);

}