//
// Created by ale on 02-07-20.
//

#ifndef RRPAIR_REPAIRRECURSIVE_H
#define RRPAIR_REPAIRRECURSIVE_H


#include <string>
#include <chrono>
#include "CLogger.h"
#include "macros.h"

namespace big_repair{

    template <

            typename C  // config object
    >
    class RePairRecursive {


    public:
        RePairRecursive(){  config = nullptr; }
        RePairRecursive(const C& c) {
            config = c;
        }
        RePairRecursive(const RePairRecursive & R) {
            this->config = R.config ;
        }
        ~RePairRecursive() = default;


        /**
        * Compress the file using a recursively version of repair
        */
        void apply(){


#ifdef MEASURE_TIME
            {
            auto start = std::chrono::high_resolution_clock::now();
#endif
            while(!config.stopCondition())
            {
                /**
                * partitioner method must create two files file_dicc and file_parse as integers
                * and Preprocess  dicc file in order to be able apply repair
                * */


                    config.parseIt();


            }
#ifdef MEASURE_TIME
            auto end = std::chrono::high_resolution_clock::now();
            auto elapse = std::chrono::duration_cast<std::chrono::microseconds>( end - start).count();
            CLogger::GetLogger()->model["total:repair:time"] = elapse;

        }
#endif
            /**
             * Apply repair to all files
             * */
#ifdef MEASURE_TIME
            {
            auto start = std::chrono::high_resolution_clock::now();
#endif
            config.compressor();
#ifdef MEASURE_TIME
            auto end = std::chrono::high_resolution_clock::now();
                auto elapse = std::chrono::duration_cast<std::chrono::microseconds>( end - start).count();
                CLogger::GetLogger()->model["total:repair:time"] = elapse;

            }
#endif


#ifdef MEASURE_TIME
            {
            auto start = std::chrono::high_resolution_clock::now();
#endif
            /**
                   * Posprocessing files
                   */

            config.postprocess();

#ifdef MEASURE_TIME
            auto end = std::chrono::high_resolution_clock::now();
                auto elapse = std::chrono::duration_cast<std::chrono::microseconds>( end - start).count();
                CLogger::GetLogger()->model["total:posprocessing:time"] = elapse;
            }
#endif
        }
        protected:
            C config;
    };




}



#endif //RRPAIR_REPAIRRECURSIVE_H
