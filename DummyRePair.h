//
// Created by ale on 02-07-20.
//

#ifndef RRPAIR_DUMMYREPAIR_H
#define RRPAIR_DUMMYREPAIR_H


#include <string>

namespace big_repair {



    class DummyRepair {

    public:
        DummyRepair() = default;
        DummyRepair(const std::string& exc){
            _exce = exc;
        }

        DummyRepair(const DummyRepair& D){
            _exce = D._exce;
        }

        DummyRepair& operator=(const DummyRepair& D){
            _exce = D._exce;
            return *this;
        }
        ~DummyRepair() = default;
        // apply compression algorithm to the file
        int apply(const std::string &file){
            std::string command_compressing= _exce +" "+ file;
            if(system(command_compressing.c_str()) < 0) return -1;
            return 1;
        }
    private:
        std::string _exce;
    };


}

#endif //RRPAIR_DUMMYREPAIR_H
