//
// Created by ale on 02-07-20.
//

#ifndef RRPAIR_HASHPARSERCONFIG_H
#define RRPAIR_HASHPARSERCONFIG_H

#include <iostream>
#include "SlidingWindow.h"
#include "HashFunction.h"
#include "RepairUtils.h"


namespace big_repair{


    template <
            typename WindowType ,
            typename HashFunction
            >
    class HashParserConfig{
        public:
            HashParserConfig() = default;
            ~HashParserConfig() = default;

            virtual WindowType* getWindow() const = 0;
            virtual uint_t mod() const = 0;
            virtual std::string inputFile() const = 0;
            virtual std::string prefixTempFiles() const = 0;
            virtual uint_t bytesToRead() const = 0;
            virtual HashFunction* hashFunction() = 0;

    };

    template <>
    class HashParserConfig<KRPSlindingWindow<>,KRPHashFunction<uint64_t ,std::string>> {


    public:
        HashParserConfig(){ }

        HashParserConfig(const HashParserConfig<KRPSlindingWindow<>,KRPHashFunction<uint64_t ,std::string>>& HC){

            _input_file = HC._input_file;
            _dir_temp = HC._dir_temp;
            _bytes_to_read = HC._bytes_to_read;
            _mod = HC._mod;
            _slinding_window = new KRPSlindingWindow<>(HC._slinding_window->sizeWindow());
            _hash_f = new KRPHashFunction<uint64_t ,std::string>();

        }

        HashParserConfig<KRPSlindingWindow<>,KRPHashFunction<uint64_t ,std::string>>& operator = (const HashParserConfig<KRPSlindingWindow<>,KRPHashFunction<uint64_t ,std::string>>& HC){
            _input_file = HC._input_file;
            _dir_temp = HC._dir_temp;
            _bytes_to_read = HC._bytes_to_read;
            _mod = HC._mod;

            _slinding_window = new KRPSlindingWindow<>(HC._slinding_window->sizeWindow());
            _hash_f = new KRPHashFunction<uint64_t ,std::string>();
            return *this;
        }
        HashParserConfig(
                const uint_t& ws,
                const uint_t& b,
                const uint_t& m,
                const std::string& input,
                const std::string& tempDir
                ):_input_file(input),_dir_temp(tempDir),_bytes_to_read(b),_mod(m)
        {
            _slinding_window = new KRPSlindingWindow<>(ws);
            _hash_f = new KRPHashFunction<uint64_t ,std::string>();
        }
        ~HashParserConfig() {
            delete _slinding_window;
        };


        std::string setInputFile(const std::string&s) {_input_file = s; return _input_file;}
        KRPSlindingWindow<>* getWindow() const { return _slinding_window;};
        uint_t mod() const { return _mod; }
        std::string inputFile() const { return _input_file; }
        std::string prefixTempFiles() const { return _dir_temp; }
        uint_t bytesToRead() const{return _bytes_to_read; }
        uint_t setBytesToRead(const uint_t& b){ _bytes_to_read = (uint_t)b; return _bytes_to_read; }
        virtual KRPHashFunction<uint64_t ,std::string>* hashFunction() { return _hash_f;}

        void print(){
//#ifdef DEBUG_PRINT
            std::cout<<"inputFile:"<<_input_file<<std::endl;
            std::cout<<"prefixTempFiles:"<<_dir_temp<<std::endl;
            std::cout<<"mod:"<<_mod<<std::endl;
            std::cout<<"w:"<<_slinding_window->sizeWindow()<<std::endl;
            std::cout<<"bytesToRead:"<<_bytes_to_read<<std::endl;
//#endif
        }

    protected:
        std::string _input_file;
        std::string _dir_temp;
        uint_t  _bytes_to_read;
        uint_t _mod;
        KRPSlindingWindow<>* _slinding_window;
        KRPHashFunction<uint64_t ,std::string>* _hash_f;

    };

}

#endif //RRPAIR_HASHPARSERCONFIG_H
