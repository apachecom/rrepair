//
// Created by ale on 02-07-20.
//

#ifndef RRPAIR_REPAIRUTILS_H
#define RRPAIR_REPAIRUTILS_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>

//#define DEBUG_PRINT

namespace big_repair{

    typedef uint32_t uint_t;
    
    namespace util{

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

    }

}



#endif //RRPAIR_REPAIRUTILS_H
