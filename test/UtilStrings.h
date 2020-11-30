//
// Created by ale on 01-07-20.
//

#ifndef RRPAIR_UTILSTRINGS_H
#define RRPAIR_UTILSTRINGS_H


#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <random>
#include <fstream>

namespace big_repair{
    namespace util{

        typedef std::vector<char> char_array;

        char_array charset()
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
        std::string random_string( size_t length, std::function<char(void)> rand_char )
        {
            std::string str(length,0);
            std::generate_n( str.begin(), length, rand_char );
            return str;
        }


        std::string generate_random_string( size_t length ){

            //0) create the character set.
            //   yes, you can use an array here,
            //   but a function is cleaner and more flexible
            const auto ch_set = util::charset();

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
            return random_string(length,randchar);

        }

        bool compareFiles(const std::string& p1, const std::string& p2) {

            std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
            std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

            if (f1.fail() || f2.fail()) {
                std::cout<<"f1.fail() || f2.fail()"<<std::endl;
                return false; //file problem
            }

            if (f1.tellg() != f2.tellg()) {
                std::cout<<"f1.tellg() != f2.tellg() "<<f1.tellg()<<" "<<f2.tellg()<<std::endl;
                return false; //size mismatch
            }

            //seek back to beginning and use std::equal to compare contents
            f1.seekg(0, std::ifstream::beg);
            f2.seekg(0, std::ifstream::beg);
            return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                              std::istreambuf_iterator<char>(),
                              std::istreambuf_iterator<char>(f2.rdbuf()));
        }



    }

}

#endif //RRPAIR_UTILSTRINGS_H
