//
// Created by via56 on 11-12-20.
//

#ifndef RRPAIR_REC_COMPRESORS_HPP
#define RRPAIR_REC_COMPRESORS_HPP

namespace rr {


    template <typename C>
    class RRePair {
        public:
            RRePair(){  config = nullptr; }
            RRePair(const C& c) {
                config = c;
            }
            RRePair(const RRePair & R) {
                this->config = R.config ;
            }
            ~RRePair() = default;
            virtual void compress() = 0;
        protected:
            C config;
    };

    template <typename C  >
    class RePairRecursive:RRePair<C> {
        public:
            RePairRecursive(): RRePair<C>(){}
            RePairRecursive(const C& c):RRePair<C>(c){}
            RePairRecursive(const RePairRecursive & R){
                this->config = R.config ;
            }
            ~RePairRecursive() = default;
            /**
            * Compress the file using a recursively version of repair
            */
            void compress(){
                while(!this->config.stopCondition())
                {
                    /**
                    * partitioner method must create two files file_dicc and file_parse as integers
                    * and Preprocess  dicc file in order to be able apply repair
                    * */
                    this->config.parseIteration();
                }
                /**
                 * Apply repair to all files
                 * */
                this->config.compressIteration();
                /**
                 * Posprocessing files
                 */
                this->config.postprocess();
            }
    };

}

#endif //RRPAIR_REC_COMPRESORS_HPP
