//
// Created by ale on 02-07-20.
//

#ifndef RRPAIR_REPAIRRECURSIVE_H
#define RRPAIR_REPAIRRECURSIVE_H


#include <string>

namespace big_repair{

    template <

            typename C  // config object
    >
    class RePairRecursive {

        RePairRecursive(){  config = nullptr; }
        RePairRecursive(C& c) { config = c;}
        RePairRecursive(const RePairRecursive & R) {
            this->config = R.config ;
        }
        ~RePairRecursive() = default;


        /**
        * Compress the file using a recursively version of repair
        */
        void apply(){
            while(config->stopCondition())
            {
                /**
                * partitioner method must create two files file_dicc and file_parse as integers
                * and Preprocess  dicc file in order to be able apply repair
                * */
                    config->parseIt();
            }
            /**
             * Apply repair to all files
             * */
            config->comppress();
            /**
             * Posprocessing files
             */
            config->postprocess();
        }
        protected:
            C* config;
    };




}



#endif //RRPAIR_REPAIRRECURSIVE_H
