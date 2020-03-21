//
// Created by alejandro on 29-07-19.
//

#ifndef RRPAIR_DUMMYPARTITION_H
#define RRPAIR_DUMMYPARTITION_H

#include <iostream>
#include <stdio.h>
#include <string.h>

#include <algorithm>
#include <map>
#include <unordered_map>
#include <vector>
#include <assert.h>
#include <fstream>
#include <chrono>
#include "CLogger.h"

#define MAX_BUFF 1024

typedef uint64_t krp_uint;


using namespace std;

// -----------------------------------------------------------------
// class to maintain a window in a string and its KR fingerprint
struct KR_window {

    int wsize;
    int *window;
    int asize;
    //const krp_uint prime = 27162335252586509;
    const krp_uint prime = 1999999973;

    krp_uint hash;

    krp_uint tot_char;

    krp_uint asize_pot;   // asize^(wsize-1) mod prime

    KR_window(int w) : wsize(w) {

        asize = 256;

        asize_pot = 1;

        for (int i = 1; i < wsize; i++)
            asize_pot = (asize_pot * asize) % prime; // ugly linear-time power algorithm
        // alloc and clear window
        window = new int[wsize];

        reset();
    }

    // init window, hash, and tot_char
    void reset() {
        for (int i = 0; i < wsize; i++) window[i] = 0;
        // init hash value and related values
        hash = tot_char = 0;
    }

    krp_uint addchar(int c) {

        int k = tot_char++ % wsize;
        // complex expression to avoid negative numbers
        hash += (prime - (window[k] * asize_pot) % prime); // remove window[k] contribution
        hash = (asize * hash + c) % prime;      //  add char i
        window[k] = c;
        // cerr << get_window() << " ~~ " << window << " --> " << hash << endl;
        return hash;
    }

    // debug only
    string get_window() {
        string w = "";
        int k = (tot_char - 1) % wsize;
        for (int i = k + 1; i < k + 1 + wsize; i++)
            w.append(1, window[i % wsize]);
        return w;
    }

    ~KR_window() {
        delete[] window;
    }

};
// -----------------------------------------------------------




class DummyPartition {

    std::string cmd_parser;
    uint size_w;
    uint mod;
    bool verbose;
    uint _len_parse;
    std::map<uint, krp_uint> rank;

public:

    DummyPartition(const std::string &c) {
        cmd_parser = c;
        size_w = 10;
        mod = 100;
        verbose = false;

    }

    DummyPartition(const uint &_w = 10, const uint &_m = 100, const std::string &c = "") {
        cmd_parser = c;
        size_w = _w;
        mod = _m;
        verbose = false;
    }

    ~DummyPartition() {}

    uint partition(const std::string &file, const int &n_bytes) const {

        std::string command_parsing =
                "../" + cmd_parser + " " + file + " -w " + std::to_string(size_w) + " -p " + std::to_string(mod) + " " +
                (verbose ? " -v " : " ");
#ifdef DEBUG_LOG_M
        std::cout << "cmd:" << command_parsing << std::endl;
#endif
        if (std::system(command_parsing.c_str()) < 0) return -1;

//        std::string cmd_rename = "mv "+file+".dict "+ file + "_dicc";
//        std::cout<<cmd_rename<<std::endl;
//        if(std::system(cmd_rename.c_str()) < 0) return -1;

        std::string cmd_rename = "mv " + file + ".parse " + file + "_parse";
#ifdef DEBUG_LOG_M
        std::cout << cmd_rename << std::endl;
#endif
        if (std::system(cmd_rename.c_str()) < 0) return -1;

        fstream fdict(file + ".dict", std::ios::in);
        fstream fdicc(file + "_dicc", std::ios::out);
        int c = 0;
        uint wc = 0;
        while (!fdict.eof() && fdict.read((char *) &c, 1)) {

            if (c == 2)continue;

            if (c < 2) {
                uint t = 0;
                fdicc.write((char *) &t, 4);
                ++wc;
#ifdef DEBUG_LOG_M
                if (wc < 10)
                    std::cout << '\n';
#endif
            } else {
#ifdef DEBUG_LOG_M
                if (wc < 10)
                    std::cout << c;
#endif
                fdicc.write((char *) &c, 4);
            }
            c = 0;
        }

        std::streampos fparse_len = 0;
        fstream fp(file + "_parse", std::ios::in);
        fp.seekg(0, std::ios_base::end);
        fparse_len = fp.tellg() - fparse_len;

        std::string cmd_rm = "rm " + file + ".dict ";
#ifdef DEBUG_LOG_M
        std::cout << cmd_rm << std::endl;
#endif
        if (std::system(cmd_rm.c_str()) < 0) return 0;
        std::cout << "END PARTITION parser(" << (uint) fparse_len / 4 << ")\n";
        std::cout << "END PARTITION dict(" << wc << ")\n";
        return (uint) fparse_len / 4;
    }

    /**
     * file is the dir of the file
     * rw number of bytes to write
     * rb number of bytes to read
    */

    uint parse_byte_file(const std::string &file) {

        fstream ffile(file, std::ios::in);
//        fstream ffileh(file+"_h",std::ios::out);
        fstream ffiled(file + "_dicc", std::ios::out);
        fstream ffilep(file + "_parse", std::ios::out);
        _len_parse = 0;
        /**
             * map to prevent colisions
            */
        std::unordered_map<krp_uint, std::pair<std::string, uint>> M;
        /**
         * current phrase
        */
        std::string word;
        std::vector<uint> P(MAX_BUFF, 0);
        uint len_parse = 0;

        // init empty KR window: constructor only needs window size
        KR_window krw(size_w);

        if (!ffile.is_open()) {
            std::cout << "Error opening the file: " << file << std::endl;
            return 0;
        }
        if (!ffilep.is_open()) {
            std::cout << "Error opening the file: " << file << std::endl;
            return 0;
        }

//        if(!ffileh.is_open()){
//            std::cout<<"Error opening the file: "<<file+"_h"<<std::endl;
//            return 0;
//        }

        if (!ffiled.is_open()) {
            std::cout << "Error opening DICC file:" << file + "_dicc" << std::endl;
            return 0;
        }

        char c;
        //std::cout<<"\treading phrases\n";

        size_t cont = 0;
        size_t bytes_dicc = 0;
#ifdef DEBUG_LOG_M
        auto start = std::chrono::steady_clock::now();
#endif
        while (!ffile.eof() && ffile.read(&c, 1)) {
            ++cont;
            if (c < 1) {
                std::cout << "Invalid symbol:" << c << std::endl;
                return 0;
            }

            word.append(&c);

            krp_uint hash = krw.addchar((int) c);
            if (hash % mod == 0 && word.size() >= size_w) {
                if (!check_word_and_write(word, P, len_parse, ffilep, M, 1))
                    return 0;
                krw.reset();
            }

        }
        /**
         * Adding last word
        */
        //std::cout<<"\tAdding last word\n";
        if (word.size())
            if (!check_word_and_write(word, P, len_parse, ffilep, M, 1))
                return 0;

        if (P.size() > 0)
            ffilep.write((char *) &P[0], len_parse * sizeof(uint));
        P.clear();

        _len_parse += len_parse;

#ifdef DEBUG_LOG_M
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>
                (std::chrono::steady_clock::now() - start);
#endif
//        ffileh.close();

        /**
         * Compute inverse rank to remap the parser file later
         * out ids start in 1
        */
//        std::unordered_map<krp_uint,uint> rank;
//        uint i = 0;
        /**
         * Writing dicc
        */
#ifdef DEBUG_LOG_M
        std::cout << "End parsing words ...(" << duration.count() << ")\n";
        uint64_t it = CLogger::GetLogger()->model["calls_to_partitioner"]++;
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_len_dicc"] = M.size();
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_windows_size"] = size_w;
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_m_mod"] = mod;
        start = std::chrono::steady_clock::now();
        size_t dicc_size = 0;
#endif


        for (auto &&item:rank) {
//            rank[item.first] = ++i;
            auto pph = M[item.second];
            uint t = 0;
            for (auto &&s_i:pph.first) {
                uint ts = 0;
                ts = (uint) s_i;
                ffiled.write((char *) &ts, 4);
            }
            //std::cout<<i<<":"<<item.second<<std::endl;
            //ffiled.write(item.second.c_str(),item.second.size());
            //ffiled.write(item.second.c_str(),item.second.size());

            ffiled.write((char *) &t, 4);
#ifdef DEBUG_LOG_M
            dicc_size += pph.first.size();
#endif

        }
#ifdef DEBUG_LOG_M
        duration = std::chrono::duration_cast<std::chrono::microseconds>
                (std::chrono::steady_clock::now() - start);
        std::cout << "Writing dict words ...(" << duration.count() << ")\n";
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_total_size_dicc"] = dicc_size;
#endif
//        ffiled.close();
//        ffilep.close();
        std::cout << "Dictionary size:" << M.size() << std::endl;
        M.clear();
        /**
         * Remaping parser file
        */
        //std::cout<<"\tWriting parser file .... \n";

//        fstream ffilehin(file+"_h",std::ios::in);
//
//        krp_uint hash;
//        uint cnt = 0;
//        size_t len_parser = 0, size_parser = 0;
//
//
//        while(!ffilehin.eof()&&ffilehin.read((char*)&hash,sizeof(krp_uint))){
//
//            auto ite = rank.find(hash);
//            if(ite == rank.end())
//            {
//                std::cout<<"Error in rank file\n";
//                return 0;
//            }
//
//            ++len_parser;
//            size_parser += sizeof(int);
//
//            int rr = ite->second;
////            std::cout<<rr<<" ";
//            ffilep.write((char*)&rr,sizeof(int));
//            ++cnt;
//
//        }
//
////        std::cout<<"\n";
////        CLogger::GetLogger()->model["it_"+std::to_string(it)+"_len_parser"] = len_parser;
////        CLogger::GetLogger()->model["it_"+std::to_string(it)+"_total_size_parse"] = size_parser;
//
//        ffilep.close();
//        ffilehin.close();


        return _len_parse;

    }

    uint parse_file(
            const std::string &file,
            const uint rb) {
        /**
       * Open files
      */
        fstream ffile(file, std::ios::in);
//            fstream ffileh(file+"_h",std::ios::out);
        fstream ffiled(file + "_dicc", std::ios::out);
        fstream ffilep(file + "_parse", std::ios::out);


        _len_parse = 0;
        /**
         * map to prevent colisions
        */
        std::unordered_map<krp_uint, std::pair<std::pair<char *, uint>, uint> > M;
        /**
         * current phrase
        */
        std::vector<int> word;
        std::vector<uint> P(MAX_BUFF, 0);
        uint len_parse = 0;
        // init empty KR window: constructor only needs window size
        KR_window krw(size_w);

        if (!ffile.is_open()) {
            std::cout << "Error opening the file: " << file << std::endl;
            return 0;
        }

        if (!ffilep.is_open()) {
            std::cout << "Error opening the file: " << file + "_parse" << std::endl;
            return 0;
        }

        if (!ffiled.is_open()) {
            std::cout << "Error opening DICC file:" << file + "_dicc" << std::endl;
            return 0;
        }

        int c = 0;
        //std::cout<<"\treading phrases\n";

        size_t cont = 0;
        size_t bytes_dicc = 0;


        auto start = std::chrono::steady_clock::now();

        while (!ffile.eof() && ffile.read((char *) &c, rb)) {

            ++cont;

            if (c < 1) {
                std::cout << "Invalid symbol:" << c << std::endl;
                return 0;
            }

            word.push_back(c);

            krp_uint hash = krw.addchar(c);

            if (hash % mod == 0 && word.size() >= size_w) {
                if (!check_word_and_write(word, P, len_parse, ffilep, M, rb))
                    return 0;
                krw.reset();
            }

        }
        /**
         * Adding last word
        */
        //std::cout<<"\tAdding last word\n";
        if (word.size())
            if (!check_word_and_write(word, P, len_parse, ffilep, M, rb))
                return 0;


        if (P.size() > 0)
            ffilep.write((char *) &P[0], len_parse * sizeof(uint));
        P.clear();

        _len_parse += len_parse;

        ffilep.close();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>
                (std::chrono::steady_clock::now() - start);

//            std::cout<<"phrases:"<<M.size()<<" len:"<<bytes_dicc/sizeof(uint)<<" bytes:"<<bytes_dicc<<" total:"<<cont<<"  (\%"<<(bytes_dicc*1.0/cont)<<")"<<std::endl;    

        std::cout << "End parsing words ...(" << duration.count() << ")\n";
        /**
         * Compute inverse rank to remap the parser file later
         * out ids start in 1
        */
        //std::cout<<"\tCompute inverse rank to remap the parser file later\n";

//            std::unordered_map<krp_uint,uint> rank;
//            uint i = 0;
        /**
         * Writing dicc
        */
        ///std::cout<<"\tWriting dicc .... \n";

        uint64_t it = CLogger::GetLogger()->model["calls_to_partitioner"]++;
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_len_dicc"] = M.size();
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_windows_size"] = size_w;
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_m_mod"] = mod;
        start = std::chrono::steady_clock::now();
        size_t dicc_size = 0;

        for (auto &&item:rank) {
//                    rank[item.first] = ++i;
            uint t = 0;
            auto pph = M[item.second];
            ffiled.write(pph.first.first, pph.first.second);
            ffiled.write((char *) &t, 4);

            dicc_size += pph.first.second;
            delete pph.first.first;
            pph.first.first = NULL;
        }

        duration = std::chrono::duration_cast<std::chrono::microseconds>
                (std::chrono::steady_clock::now() - start);
        std::cout << "Writing dict words ...(" << duration.count() << ")\n";

        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_total_size_dicc"] = dicc_size;


        ffiled.close();
        std::cout << "Dictionary size:" << M.size() << std::endl;
        M.clear();
        /**
         * Remaping parser file
        */
        //std::cout<<"\tWriting parser file .... \n";

//            fstream ffilehin(file+"_h",std::ios::in);
//
//            krp_uint hash;
//            uint cnt = 0;
//            size_t len_parser = 0, size_parser = 0;
//
//
//            while(!ffilehin.eof()&&ffilehin.read((char*)&hash,sizeof(krp_uint))){
//
//                if(rank.find(hash) == rank.end())
//                {
//                    std::cout<<"Error in rank file\n";
//                    return 0;
//                }
//
//                ++len_parser;
//                size_parser += sizeof(int);
//
//                int rr = rank[hash];
////                std::cout<<rr<<" ";
//                ffilep.write((char*)&rr,sizeof(int));
//                ++cnt;
//
//            }
        //std::cout<<std::endl;

        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_len_parser"] = _len_parse;
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_total_size_parse"] = dicc_size;

        ffilep.close();
//            ffilehin.close();

//            std::string rm_cmd = "rm "+file+"_h";
//
//            if(std::system(rm_cmd.c_str()) < 0){
//                std::cout<<"Unable to remove the temporary file :"<<file+"_h"<<std::endl;
//                std::cout<<rm_cmd<<std::endl;
//            }

        return _len_parse;
    }

    /**
     * file is the dir of the file
     * rw number of bytes to write
     * rb number of bytes to read
    */

    uint parse_byte_fileb(const std::string &file) {

        fstream ffile(file, std::ios::in);
        fstream ffileh(file + "_h", std::ios::out);
        fstream ffiled(file + "_dicc", std::ios::out);
        fstream ffilep(file + "_parse", std::ios::out);

        /**
             * map to prevent colisions
            */
        std::unordered_map<krp_uint, std::string> M;
        /**
         * current phrase
        */
        std::string word;

        // init empty KR window: constructor only needs window size
        KR_window krw(size_w);

        if (!ffile.is_open()) {
            std::cout << "Error opening the file: " << file << std::endl;
            return 0;
        }

        if (!ffileh.is_open()) {
            std::cout << "Error opening the file: " << file + "_h" << std::endl;
            return 0;
        }

        if (!ffiled.is_open()) {
            std::cout << "Error opening DICC file:" << file + "_dicc" << std::endl;
            return 0;
        }

        char c;
#ifdef DEBUG_LOG_M
        
        uint64_t it = CLogger::GetLogger()->model["calls_to_partitioner"];
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_memory_peak_dicc"] = 0;
        ffile.seekg(0, std::ios_base::end);
        size_t bytes_file =ffile.tellg();
        std::cout<<"file size:"<<bytes_file<<"(bytes)\n";
        std::cout<<"\treading phrases\n";
        ffile.seekg(0, std::ios_base::beg);
        ffile.clear();
#endif

        size_t cont = 0;
        size_t bytes_dicc = 0;

        while (!ffile.eof() && ffile.read(&c, 1)) {

            ++cont;
            if (c < 1) {
                std::cout << "Invalid symbol:" << c << std::endl;
                return 0;
            }

            word.append(&c);
            krp_uint hash = krw.addchar((int) c);
            if (hash % mod == 0 && word.size() >= size_w) {
                if (!check_word_and_write(word, ffileh, ffiled, M, 1)){
                    #ifdef DEBUG_LOG_M
                        std::cout<<"returning false in check word\n";
                    #endif
                    return 0;
                }                    
                krw.reset();
                #ifdef DEBUG_LOG_M
                    if((cont)%10000 == 0)
                        std::cout<<"\rcurrent size dicc ("<<M.size()<<")..."<<cont*100.0/bytes_file<<"\% Memory("<<CLogger::GetLogger()->model["it_" + std::to_string(it) + "_memory_peak_dicc"]/1000000000.0<<")";
                #endif
            }
           

        }
#ifdef DEBUG_LOG_M
      std::cout<<std::endl;
#endif
        /**
         * Adding last word
        */
        //std::cout<<"\tAdding last word\n";
        if (word.size())
            if (!check_word_and_write(word, ffileh, ffiled, M, 1)){
                #ifdef DEBUG_LOG_M
                        std::cout<<"returning false in check word\n";
                #endif
                return 0;
            }


        ffileh.close();
#ifdef DEBUG_LOG_M
        std::cout<<std::endl;
        std::cout<<"End reading...\n";
#endif

        /**
         * Compute inverse rank to remap the parser file later
         * out ids start in 1
        */
        std::unordered_map<krp_uint, uint> rank;
        uint i = 0;
        /**
         * Writing dicc
        */
#ifdef DEBUG_LOG_M
        //uint64_t it = CLogger::GetLogger()->model["calls_to_partitioner"]++;
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_len_dicc"] = M.size();
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_windows_size"] = size_w;
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_m_mod"] = mod;

        size_t iiii = 0;
        
#endif
        size_t dicc_size = 0;

        for (auto &&item:M) {
            rank[item.first] = ++i;
            uint t = 0;

            for (auto &&s_i:item.second) {
                uint ts = 0;
                ts = (uint) s_i;
                ffiled.write((char *) &ts, 4);
            }
//            std::cout<<i<<":"<<item.second<<std::endl;
            //ffiled.write(item.second.c_str(),item.second.size());
            ffiled.write((char *) &t, 4);
            dicc_size += item.second.size();
#ifdef DEBUG_LOG_M
            std::cout<<"\rwritting dicc file ("<<iiii*100.0/M.size()<<")...";
#endif

        }
#ifdef DEBUG_LOG_M
        std::cout<<std::endl;
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_total_size_dicc"] = dicc_size;

#endif

        ffiled.close();
//        std::cout<<"Dictionary size:"<<M.size()<<std::endl;
        M.clear();
        /**
         * Remaping parser file
        */
        //std::cout<<"\tWriting parser file .... \n";

        fstream ffilehin(file + "_h", std::ios::in);
#ifdef DEBUG_LOG_M



        ffilehin.seekg(0, std::ios_base::end);
        size_t bytes_file_h =ffilehin.tellg()/sizeof(uint64_t);
        std::cout<<"file parser len:"<<bytes_file_h<<"(bytes)\n";
        std::cout<<"\treading phrases\n";
        ffilehin.seekg(0, std::ios_base::beg);
        ffilehin.clear();
#endif
        krp_uint hash;
        uint cnt = 0;
        size_t len_parser = 0, size_parser = 0;


        while (!ffilehin.eof() && ffilehin.read((char *) &hash, sizeof(krp_uint))) {

            auto ite = rank.find(hash);
            if (ite == rank.end()) {
                std::cout << "Error in rank file\n";
                return 0;
            }

            ++len_parser;
            size_parser += sizeof(int);

            int rr = ite->second;
//            std::cout<<rr<<" ";
            ffilep.write((char *) &rr, sizeof(int));
            ++cnt;
#ifdef DEBUG_LOG_M
            std::cout<<"\rwritting parse file ("<<cnt*100.0/bytes_file_h<<")...";
#endif
        }
        std::cout << "\n";
#ifdef DEBUG_LOG_M
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_len_parser"] = len_parser;
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_total_size_parse"] = size_parser;
#endif
        ffilep.close();
        ffilehin.close();

        std::string rm_cmd = "rm "+file+"_h";

        if(std::system(rm_cmd.c_str()) < 0){
            std::cout<<"Unable to remove the temporary file :"<<file+"_h"<<std::endl;
            std::cout<<rm_cmd<<std::endl;
        }

        return cnt;

    }

    uint parse_fileb(
            const std::string &file,
            const uint rb) {
        /**
         * Open files
        */
        fstream ffile(file, std::ios::in);
        fstream ffileh(file + "_h", std::ios::out);
        fstream ffiled(file + "_dicc", std::ios::out);
        fstream ffilep(file + "_parse", std::ios::out);
        /**
         * map to prevent colisions
        */
        std::unordered_map<krp_uint, std::pair<char *, uint>> M;
        /**
         * current phrase
        */
        std::vector<int> word;

        /**
         * init empty KR window: constructor only needs window size
         * */
        KR_window krw(size_w);

        if (!ffile.is_open()) {
            std::cout << "Error opening the file: " << file << std::endl;
            return 0;
        }

        if (!ffileh.is_open()) {
            std::cout << "Error opening the file: " << file + "_h" << std::endl;
            return 0;
        }

        if (!ffiled.is_open()) {
            std::cout << "Error opening DICC file:" << file + "_dicc" << std::endl;
            return 0;
        }

        int c = 0;
#ifdef DEBUG_LOG_M
        std::cout<<"\treading phrases\n";
#endif
        size_t cont = 0;
        size_t bytes_dicc = 0;

        while (!ffile.eof() && ffile.read((char *) &c, rb)) {

            ++cont;

            if (c < 1) {
                std::cout << "Invalid symbol:" << c << std::endl;
                return 0;
            }

            word.push_back(c);

            krp_uint hash = krw.addchar(c);

            if (hash % mod == 0 && word.size() >= size_w) {
                if (!check_word_and_write(word, ffileh, ffiled, M, rb))
                    return 0;

                krw.reset();
            }

        }
        /**
         * Adding last word
        */
        //std::cout<<"\tAdding last word\n";
        if (word.size())
            if (!check_word_and_write(word, ffileh, ffiled, M, rb))
                return 0;

        ffileh.close();
#ifdef DEBUG_LOG_M
        std::cout<<"End reading...\n";
#endif
        /**
         * Compute inverse rank to remap the parser file later
         * out ids start in 1
        */
        //std::cout<<"\tCompute inverse rank to remap the parser file later\n";

        std::unordered_map<krp_uint, uint> rank;
        uint i = 0;
        /**
         * Writing dicc
        */
        ///std::cout<<"\tWriting dicc .... \n";
#ifdef DEBUG_LOG_M
        uint64_t it = CLogger::GetLogger()->model["calls_to_partitioner"]++;
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_len_dicc"] = M.size();
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_windows_size"] = size_w;
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_m_mod"] = mod;
#endif

        size_t dicc_size = 0;

        for (auto &&item:M) {
            rank[item.first] = ++i;
            uint t = 0;
            ffiled.write(item.second.first, item.second.second);
            ffiled.write((char *) &t, 4);

            dicc_size += item.second.second;
            delete item.second.first;
            item.second.first = NULL;
        }
#ifdef DEBUG_LOG_M
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_total_size_dicc"] = dicc_size;
#endif

        ffiled.close();
//            std::cout<<"Dictionary size:"<<M.size()<<std::endl;
        M.clear();
        /**
         * Remaping parser file
        */
        //std::cout<<"\tWriting parser file .... \n";

        fstream ffilehin(file + "_h", std::ios::in);

        krp_uint hash;
        uint cnt = 0;
        size_t len_parser = 0, size_parser = 0;


        while (!ffilehin.eof() && ffilehin.read((char *) &hash, sizeof(krp_uint))) {

            if (rank.find(hash) == rank.end()) {
                std::cout << "Error in rank file\n";
                return 0;
            }

            ++len_parser;
            size_parser += sizeof(int);

            int rr = rank[hash];
            ffilep.write((char *) &rr, sizeof(int));
            ++cnt;
        }

#ifdef DEBUG_LOG_M
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_len_parser"] = len_parser;
        CLogger::GetLogger()->model["it_" + std::to_string(it) + "_total_size_parse"] = size_parser;
#endif
        ffilep.close();
        ffilehin.close();

        std::string rm_cmd = "rm " + file + "_h";

        if (std::system(rm_cmd.c_str()) < 0) {
            std::cout << "Unable to remove the temporary file :" << file + "_h" << std::endl;
            std::cout << rm_cmd << std::endl;
        }

        return cnt;
    }


private:

    /** 
     * compute 64-bit KR hash of a string 
     * to avoid overflows in 64 bit aritmethic the prime is taken < 2**55
    */
    krp_uint kr_hash(string s) const {
        krp_uint hash = 0;
        //const uint64_t prime = 3355443229;     // next prime(2**31+2**30+2**27)
        const krp_uint prime = 27162335252586509; // next prime (2**54 + 2**53 + 2**47 + 2**13)

        for (size_t k = 0; k < s.size(); k++) {
            int c = (unsigned char) s[k];
            assert(c >= 0 && c < 256);

            hash = (256 * hash + c) % prime;    //  add char k
        }
        return hash;
    }

    krp_uint kr_hash(const char *s, uint len) const {
        //std::cout<<"computing hash\n"<<std::endl;
        krp_uint hash = 0;
        //const uint64_t prime = 3355443229;     // next prime(2**31+2**30+2**27)
        const krp_uint prime = 27162335252586509; // next prime (2**54 + 2**53 + 2**47 + 2**13)

        for (size_t k = 0; k < len; k++) {

            int c = (unsigned int) s[k];
            //  std::cout<<c<<" ";
            assert(c >= 0 && c < 256);
            hash = (256 * hash + c) % prime;    //  add char k
        }
        //std::cout<<std::endl;

        return hash;
    }

    /**
     * 
     * check hash and write in the tmeporary parser file 
    */
    bool check_word_and_write
            (

                    std::vector<int> &word,
                    std::vector<uint> &P,
                    uint &plen,
                    std::fstream &ffilep,
                    std::unordered_map<krp_uint, std::pair<std::pair<char *, uint>, uint> > &M,
                    const uint &bytes

            ) {
        uint len = word.size();
        int *A = new int[len];

        std::copy(word.begin(), word.end(), A);
        word.clear();
        char *S = (char *) A;

        krp_uint word_hash = kr_hash(S, len * 4);
        auto it = M.find(word_hash);

        if (it == M.end()) {
            M[word_hash] = std::make_pair(std::make_pair(S, len * 4), M.size() + 1);
            rank[M.size()] = word_hash;
            P[plen] = M.size();
        } else {
            ///////////////CHECK COLISSION/////////////////
            if (memcmp(it->second.first.first, S, len * 4) != 0) {
                std::cout << "DICC SIZE" << M.size() << std::endl;
                std::cout << "COLISION PROBLEM\n";
                delete S;
                return false;
            }

            P[plen] = it->second.second;
            delete S;
        }
        ++plen;

        if (plen >= MAX_BUFF) {
            _len_parse += plen;
            ffilep.write((char *) &P[0], plen * sizeof(uint));
            //               P.clear();
            plen = 0;
        }

        word.clear();
//                    ffileh.write((char*)&word_hash,sizeof(krp_uint));

        return true;
    }


    /**
     *
     * check hash and write in the tmeporary parser file
    */
    bool check_word_and_write
            (

                    std::string &word,
                    std::vector<uint> &P,
                    uint &plen,
                    std::fstream &ffilep,
                    std::unordered_map<krp_uint, std::pair<std::string, uint>> &M,
                    const uint &bytes

            ) {


        uint len = word.size();
        if (len < size_w)
            return false;

        krp_uint word_hash = kr_hash(word.c_str(), len);

        auto it = M.find(word_hash);

        if (it == M.end()) {
            M[word_hash] = std::make_pair(word, M.size() + 1);
            rank[M.size()] = word_hash;
            P[plen] = M.size();
        } else {
            ///////////////CHECK COLISSION/////////////////
            if (it->second.first != word) {
                std::cout << "DICC SIZE" << M.size() << std::endl;
                std::cout << "COLISION PROBLEM\n";
            }

            P[plen] = it->second.second;
        }
        ++plen;
        //ffileh.write((char*)&word_hash,sizeof(krp_uint));
        if (plen >= MAX_BUFF) {
            _len_parse += plen;
            ffilep.write((char *) &P[0], plen * sizeof(uint));
            // P.clear();
            plen = 0;
        }
        word.clear();
        return true;
    }

    /**
    *
    * check hash and write in the tmeporary parser file
   */
    bool check_word_and_write
            (

                    std::vector<int> &word,
                    fstream &ffileh, fstream &ffiled,
                    std::unordered_map<krp_uint, std::pair<char *, uint>> &M,
                    const uint &bytes

            ) {
        uint len = word.size();
        int *A = new int[len];
        std::copy(word.begin(), word.end(), A);
        word.clear();
        char *S = (char *) A;
        krp_uint word_hash = kr_hash(S, len * 4);

        auto it = M.find(word_hash);
        if (it == M.end()) {
            M[word_hash] = std::make_pair(S, len * 4);
        } else {
            ///////////////CHECK COLISSION/////////////////
            if (memcmp(it->second.first, S, len * 4) != 0) {
                std::cout << "DICC SIZE" << M.size() << std::endl;
                std::cout << "COLISION PROBLEM\n";
                std::cout << "Lens:" << len << " " << M[word_hash].second / 4 << "\n";
                std::cout << "Hash:" << kr_hash(S, len * 4) << " " << kr_hash(M[word_hash].first, M[word_hash].second)
                          << "\n";
                std::cout << "Hash:" << word_hash << std::endl;
                std::cout << "S1:";

                int *ptr = (int *) S;

                for (size_t i = 0; i < len; i++)
                    printf("%c", ptr[i]);
                printf("\n");

                ptr = (int *) M[word_hash].first;

                std::cout << "S2:";
                for (size_t i = 0; i < M[word_hash].second / 4; i++)
                    printf("%c", ptr[i]);
                printf("\n");

                delete S;
                return false;
            }
            delete S;
        }

        ffileh.write((char *) &word_hash, sizeof(krp_uint));

        return true;
    }


    /**
     *
     * check hash and write in the tmeporary parser file
    */
    bool check_word_and_write
            (

                    std::string &word,
                    fstream &ffileh, fstream &ffiled,
                    std::unordered_map<krp_uint, std::string> &M,
                    const uint &bytes

            ) {


        uint len = word.size();
        /* if (len < size_w)
            return false;
 */
        krp_uint word_hash = kr_hash(word.c_str(), len);

        auto it = M.find(word_hash);

        if (it == M.end()) {
            M[word_hash] = word;
            
            #ifdef DEBUG_LOG_M
                uint64_t it = CLogger::GetLogger()->model["calls_to_partitioner"];
                CLogger::GetLogger()->model["it_" + std::to_string(it) + "_memory_peak_dicc"] += word.size();
            #endif

        } else {
            ///////////////CHECK COLISSION/////////////////
            if (it->second != word) {
                std::cout << "DICC SIZE" << M.size() << std::endl;
                std::cout << "COLISION PROBLEM\n";
            }
        }

        ffileh.write((char *) &word_hash, sizeof(krp_uint));

        word.clear();

        return true;
    }

    void write_phrase_char_buffer(const std::vector<uint> &word, char *str, const uint &bytes) {
        str = new char[bytes * word.size()];
        uint pos = 0;
        for (auto &&item:word) {
            uint c = item;
            for (uint i = 0; i < bytes; ++i) {
                str[pos] = c >> (8 * i);
                ++pos;
            }
        }
    }



};


#endif //RRPAIR_DUMMYPARTITION_H
