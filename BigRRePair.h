//
// Created by alejandro on 29-07-19.
//

#ifndef RRPA_BIGRREPAIR_H
#define RRPAIR_BIGRREPAIR_H



#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <chrono>
#include "include/logger/CLogger.h"



#define RR_SIZE 10000000

class BigRRePair {


public:

    std::string filename;
    uint symbols;
    uint bytes_per_symbol;
    std::fstream fc_out;
    std::fstream fr_out;
    std::set<uint> map_sigma;

    BigRRePair() {
        symbols = 0;
    }

    BigRRePair(const std::string &fdir, const int& bytes = 1){

        filename = fdir;
        bytes_per_symbol = bytes;

        std::fstream f(filename,std::ios::in);
        if(!f.is_open())
        {
            filename = "";
            symbols = 0;
            bytes_per_symbol = 0;
            return ;

        }

    }

    virtual ~BigRRePair() {}
  

protected:

    /*
     * Take a sequence S of variables and create a dummy binary grammar
     * for S
     * Write in output_file
     * */
    void create_binary_grammars(const std::vector<int>& R)
    {

        std::vector<int>Q(R.size());
        std::copy(R.begin(),R.end(),Q.begin());
        while(Q.size() > 1)
        {
            fr_out.write((char*)&Q[0],sizeof(int));
            fr_out.write((char*)&Q[1],sizeof(int));

            auto it = R.begin();
            Q.erase(it,it+1);
            Q.push_back(++symbols);
        }
    }

    /*
     * true if X is prefix of Y false ioc
     *
     * */

    bool prefix_int(std::vector<int> & X,std::vector<int> & Y){

        if(Y.size() < X.size())
            return false;


        for (int i = 0; i < X.size(); ++i) {
            if(X[i]!=Y[i])return false;
        }

        return true;
    }
    public:
    std::pair<uint,uint> preprocess_dicc(std::fstream& fdicc,std::fstream & out, const uint& bytes = 1)
    {
        
        /*
         *
         * Prepare dicc to compression
         * insert (phrase-1) distinct symbols between frases
         * create a new file with the new sequence ready to compress
         *
         * */

        if(!fdicc.is_open())
        {
            std::cout<<"ERROR PREPROCESSING DICC\n";
            return std::make_pair(0,0);  
        }
        
        uint n_words = 0;
        std::vector<uint> current_ph;
        std::unordered_map< uint, std::vector<uint> > dicc ;
        uint _max = 0;
        
        uint c = 0;
        while (!fdicc.eof() && fdicc.read((char*)&c,4))
            _max = (_max < c)?c:_max;

        fdicc.clear();
        fdicc.seekg(0,fdicc.beg);

        uint m = _max; c = 0;

        while (!fdicc.eof() && fdicc.read((char*)&c,4))
        {   
            if(c < 0){
                std::cout<<"ERROR CARACTER NO PERMITIDO "<<c<<"\n";
                return std::make_pair(0,0);
            }

            if(c == 0){
                ++m;
                out.write((char*)&m,4);
            }else{
                out.write((char*)&c,4);
            }

            

        }
//        std::cout<<"m:"<<m<<"sizeof(uint):"<<sizeof(uint)<<std::endl;
        /* std::cout<<"sigma size:"<<sigma_set.size()<<"\n";
        for(auto && cc:sigma_set)
            std::cout<<cc<<"|"<<char(cc)<<std::endl;
          

        std::cout<<"DICC size:"<<dicc.size()<<"\n"; */
        
        //uint mmm = _max;
        /* for(uint ii = 0; ii < dicc.size(); ii++){
            
            auto word = std::make_pair(ii+1,dicc[ii+1]);

            int* t_int = new int[word.second.size()+1];
            std::copy(word.second.begin(),word.second.end(),t_int);
            t_int[word.second.size()] = ++mmm;
            out.write((char*)t_int,(word.second.size()+1)*sizeof(int));
        } */

        /**(max,mmm]*/
        //return std::make_pair(_max,mmm);
        return std::make_pair(_max,m);

    }
    /***
     * 
     * Combine al grammars in only one
     */
    

    size_t posprocessing_files(const std::string &file, const uint &iterations, const std::vector<std::pair<uint,uint>> &sigma_b){

//        std::cout<<"posprocessing_files\n";

        /** Create a file for the finals rules */
        std::fstream Gf(file+".Rr",std::ios::out|std::ios::binary);

        if(!Gf.is_open()){
            std::cout<<"error opening file: "<<file+"_rr_.R"<<std::endl;
            return 0;
        }
        /**This map the current alphabeth to the corresponding rule**/
        std::unordered_map<uint,uint> S;

        uint off_rules = 0;
        
        /**
         * iterate for all files file_it_rr.C and file_it_rr.R
        */
        size_t total_size = 0;
    
        for(size_t i = 0; i < iterations; i++)
        {
            /* keep update the number of rules */
            uint n_rules = 0;
            /*current file .R*/
            std::string curr_file = file + "_dicc_"+std::to_string(i)+"_rr.R";

            std::fstream Rf(curr_file,std::ios::in);

            if(!Rf.is_open()){
                std::cout<<"error opening file: "<<curr_file<<std::endl;
                return 0;
            }

            int maxsigma = 0;
            //std::cout<<"reading re-pair files:"<<curr_file<<std::endl;
            
            /* This return max(sigma) + 1 */
//            std::cout<<"max sigma re-pair:"<<maxsigma<<std::endl;
            int c;


            /*process original sigma **/
        
            if(i == 0){        
            /* Write sigma in the grammar file */
                while(!Rf.eof() && Rf.read((char*)&c,4)){
                    if(c <= sigma_b[i].first && S.find(c) == S.end()){
                        S[c] = c ;
                    }
                }

                Rf.clear();
                Rf.seekg(0,Rf.beg);
                Rf.read((char*)&maxsigma,sizeof(int));
                /** Set initial alphabeth in S **/
//                int gs = map_sigma.size();

              //  std::cout<<"real sigma:"<<maxsigma<<" "<<sigma_b[i].first<<std::endl;
                Gf.write((char*)&sigma_b[i].first,4);
                
                off_rules = sigma_b[i].first+1;
                
                for(auto&&ii:map_sigma)
                {   
                    uint a = ii;
                 //   Gf.write((char*)&a,4);
                    S[a] = a;
                  //  std::cout<<a<<":"<<char(a)<<std::endl;
                }
            }else{
                Rf.read((char*)&maxsigma,sizeof(int));
            }

            /**
             * Read the rules and update the ids using the current offset 
             * and the map S 
            */
            int X,Y;
            //std::cout<<"rules\n";
            size_t log_n_rules = 0;
            while(!Rf.eof() && Rf.read((char*)&X,4) && Rf.read((char*)&Y,4)){
                /* If X is an element of the alp we change it by the corresponding non-terminal**/
//                std::cout<<"\t"<<X<<" "<<Y<<std::endl;
                if(X < maxsigma)X = S[X];
                /* If X is a rule we add it the correspondingS offset**/
                else X = (X - maxsigma) + off_rules;
                /*Same for Y**/
                if(Y < maxsigma)Y = S[Y];
                else Y = (Y-maxsigma) + off_rules;
                /*Write in the file*/
                Gf.write((char*)&X,4);
                Gf.write((char*)&Y,4);
                log_n_rules++;

//                std::cout<<off_rules+n_rules<<":"<<X<<" "<<Y<<std::endl;
                /*Increase the number of rules*/
                //std::cout<<n_rules+off_rules<<"->"<<((map_sigma.find(X) == map_sigma.end())?X:char(X))<<" ";
                //std::cout<<((map_sigma.find(Y) == map_sigma.end())?Y:char(Y))<<std::endl;
                ++n_rules;            
            }

            CLogger::GetLogger()->model["it_"+std::to_string(i)+"_posproc_repair_rules"] = log_n_rules;


            //std::cout<<"Read the rules and update the ids using the current offset :"<<off_rules<<std::endl;

            /* We process the file .C creating rules for each phrase in dicc and updating the map for the alp
            *   and the number of rules.
            * */      
            //std::cout<<"We process the file .C creating rules for each phrase in dicc and updating the map for the alp :"<<sigma_b[i].second<<std::endl;
            uint t = process_first_rule(file,Gf,i,sigma_b[i],n_rules,off_rules,S,i);

                
            if(t < 0){
                std::cout<<"error processing file.C\n";
                return 0;
            }
            //std::cout<<"Update the offset of the rules adding the number of rules created in this iteration"<<std::endl;
            
            /* Update the offset of the rules adding the number of rules created in this iteration**/
            off_rules += n_rules;
            total_size += n_rules;
            //std::cout<<"off_rules:"<<off_rules<<std::endl;
            //std::cout<<"n_rules:"<<n_rules<<std::endl;

            std::string cmd = "rm "+ curr_file;
            if(system(cmd.c_str()) < 0) std::cout<<"Error deleting file: "+cmd+"\n";
            cmd = "rm " + file + "_parse_" + std::to_string(i);
            if(system(cmd.c_str()) < 0) std::cout<<"Error deleting file: "+cmd+"\n";
                
        }
        
        /**
         *  Update last parser file  
         * */
        std::cout<<"  Update last parser file  \n";
        std::fstream Cf(file+".Cr",std::ios::out);
        if(!Cf.is_open()){
            std::cout<<"error opening file: "<<file+".C"<<std::endl;
            return 0;
        }        
        auto pf = posprocessing_parse_file(file+"_parse_"+std::to_string(iterations-1),Gf,Cf,off_rules,S);

        std::string cmd = "rm "+ file+ "_*";
        if(system(cmd.c_str()) < 0) std::cout<<"Error deleting files: "+cmd+"\n";

        total_size = (total_size+pf.first)*2+pf.second;

        return total_size;
    }


    /**
     * Process the files.C to create the asocciated rules
     * file:name of the file
     * Gf fstream to the file of the grammar
     * i iteration
     * sigma_b is the value that marks the phrases of the dicctionary (X > sigma[i] the is a marker)
     * n_rules number of current rules 
     * off_rules is the current offset of the rules.
     * S is the map from the alphabet of the file to the real rule (This is updated for next iteration)
    */
    int     process_first_rule(const std::string & file,std::fstream& Gf,const uint &i,const std::pair<uint,uint> &sigma_b,
                             uint& n_rules, const uint& off_rules, std::unordered_map<uint,uint>&S,const uint &it){


        //std::cout<<"\tprocess_first_rule\n";
        std::string curr_file = file+"_dicc_"+std::to_string(i)+"_rr.C";
        std::fstream Cf(curr_file,std::ios::in);
        
        if(!Cf.is_open()){
            std::cout<<"error opening file: "<<curr_file<<std::endl;
            return -1;
        }
        /*Clear the alp S**/
        ///S.clear();
        /* store the current rule*/
        std::vector<int> X;
        /* count the number of rules created */
        uint cnt_rules = 0;
        uint curr_symbol = 0;
        int c = 0;        
        std::unordered_map<uint,uint> n_S;
        /* std::cout<<"\twhile(!Cf.eof() && Cf.read((char*)&c,4)) ("<<sigma_b.first<<","<<sigma_b.second<<"]\n";
        std::cout<<"first rule\n";
        
        std::cout<<"offset rules:"<<off_rules<<std::endl;
        std::cout<<"number rules added:"<<n_rules<<std::endl; 
        */

        CLogger::GetLogger()->model["it_"+std::to_string(it)+"_first_repair_seq_binary_rules"] = 0;
        CLogger::GetLogger()->model["it_"+std::to_string(it)+"_first_repair_len_seq"] = 0;
        while(!Cf.eof() && Cf.read((char*)&c,4))
        {


            CLogger::GetLogger()->model["it_"+std::to_string(i)+"_first_repair_len_seq"]++;

            if(c <= sigma_b.first || c > sigma_b.second){/*if it is a valid simbol add it to the current rule*/
                if(c <= sigma_b.first)
                    X.push_back(S[c]);
                else
                    X.push_back(c - sigma_b.second - 1 + off_rules);            
    //            std::cout<<((X.back() <= sigma_b.first )?(char)X.back():X.back())<<" ";
                
                
  //              std::cout<<"\t\t\t"<<c<<std::endl;
            }
            else
            {
                /*if it is a marker symbol*/

                

                /*Create the pairs grammar for the rules*/
    //            std::cout<<"\t\tCreate the pairs grammar for the rules X:"<<X.size()<<" c:"<<c<<"\n";
                /* if(X.size() == 0)
                    std::cout<<c<<std::endl;
 */     
                                
        //        std::cout<<std::endl;                
                if(X.size() > 1){
      //              sleep(1);
                    uint last = X.size()-1, i = 0;
                    while(i < X.size()-1)
                    {
                            if(i == last){
//                                std::cout<<"last:"<<i<<"--"<<X[i]<<std::endl;
                                X.push_back(X[last]);
                                last = X.size()-1;
                                ++i;    
                            }else{
                                X.push_back( n_rules + off_rules );

                                if(i+1 == last)
                                    last = X.size()-1;

  //                              std::cout<<off_rules+n_rules<<"->"<<X[i]<<","<<X[i+1]<<std::endl;
                                ++n_rules;

                                Gf.write((char*)&X[i],4);
                                Gf.write((char*)&X[i+1],4);

                                CLogger::GetLogger()->model["it_"+std::to_string(it)+"_first_repair_seq_binary_rules"]++;

                                i+=2;
                            }
                            
                    }
                    X.clear();


                    /* int last_element = X.back();
                        
                    while(X.size() != 1)
                    {
                        int X1,X2;
                        X1 = X.front();X.pop();
                        
                        if(X1 == last_element){
                            X.push(X1);
                        }                            
                        else{
                            
                            X2 = X.front();X.pop();
                            X.push( n_rules + off_rules );
                            
                            if(X2 == last_element)
                                last_element = X.back();

                            std::cout<<off_rules+n_rules<<"->"<<((X1 <= sigma_b.first )?(char)X1:X1)<<","<<((X2 <= sigma_b.first )?(char)X2:X2)<<std::endl;
                            
                            ++n_rules;

                            Gf.write((char*)&X1,4);
                            Gf.write((char*)&X2,4);
                        }

                    }
                    X.pop();
 */
                    /*
                    std::queue<int> NX;

                    while(!X.empty() || !NX.empty())
                    {
                        //until reach only one terminal
                        int X1,X2;
                        //Extract two elements and put another rule (keep n_rules updated)

                        if(X.size() > 1){
                            X1 = X.front();X.pop();
                            X2 = X.front();X.pop();
                            NX.push( n_rules + off_rules );
                        
                        }else{
                            
                            if(X.size() == 1 && NX.size() == 1 ){                            
                                X1 = X.front();X.pop();                            
                                NX.push(X1);
                                continue;

                            }else{
                                if(NX.size() == 1){
                                    NX.pop();
                                    break;
                                }                                
                                else{
                                    X1 = NX.front();NX.pop();
                                    X2 = NX.front();NX.pop();
                                    NX.push(n_rules  + off_rules);   
                                }
                            }
                        }
                        //Write the rule in the file
                        if(!NX.empty())
                        {
                            std::cout<<off_rules+n_rules<<"->"<<((X1 <= sigma_b.first )?(char)X1:X1)<<","<<((X2 <= sigma_b.first )?(char)X2:X2)<<std::endl;
                            ++n_rules;
                            Gf.write((char*)&X1,4);
                            Gf.write((char*)&X2,4);
                        }
                    } 
                    */
    //                std::cout<<"\t\tChecking size queue:"<<X.size()<<"\n";
                    
                    /**Update symbol*/
                    n_S[++curr_symbol] = n_rules - 1 + off_rules;
                }
                else
                {
                    n_S[++curr_symbol] = X.front();
                    X.clear();
                }
            }
        }

        S.clear();
        S = n_S;
        return 1;
    }

    /**
     * Compress the file using a recursively version of repair
    */
    
    template<typename GrammarCompressor, typename TextPartitioner>
    size_t rrpair(const std::string & file, GrammarCompressor & compressor, TextPartitioner & partitioner)
    {

        uint iteration  = 0;
        /**
         * First it eration we read bytes
         * parse file receive the file and the number of bytes to read.
        */
        
        uint len = RR_SIZE+1;
        
//        std::cout<<"map_sigma end...("<<map_sigma.size()<<")\n";
        


       /*This vector stores the simbols in each level*/
        std::vector<std::pair<uint,uint>> sigmas;
        std::string part_file = file;


//        CLogger::GetLogger()->model["sigma"] = map_sigma.size();


        while(len > RR_SIZE)
        {

            /**
            * partitioner method must create two files file_dicc and file_parse as integers
            * 
            * */
            std::cout<<"***********it:"<<(int)iteration<<std::endl;
            /**call partitioner return the len of the parser file*/
            auto start = std::chrono::steady_clock::now();

//            len = partitioner.parse_file(part_file,(iteration == 0)?1:4);
            if(iteration != 0){
                len = partitioner.parse_fileb(part_file,4);
            }else{
                len = partitioner.parse_byte_fileb(part_file);
            }
            
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>
                    (std::chrono::steady_clock::now() - start);

            CLogger::GetLogger()->model["it_"+std::to_string(iteration)+"_parser_time"] = duration.count();

            std::cout<<"end parsing file ...len:"<<len<<"\n";
            std::cout<<"end parsing file ...time:"<<duration.count()<<"(microseconds)\n";

//            sleep(5);
            /**rename the files */
            std::string nm_cmd_dicc = "mv "+part_file+"_dicc "+file+"_dicc_"+std::to_string(iteration);
            std::string nm_cmd_parse = "mv "+part_file+"_parse "+file+"_parse_"+std::to_string(iteration);

//            std::cout<<nm_cmd_dicc<<std::endl;
//            std::cout<<nm_cmd_parse<<std::endl;

            if(std::system(nm_cmd_dicc.c_str()) < 0 || std::system(nm_cmd_parse.c_str()) < 0 )
            {
                std::cout<<"Error we can't rename the files\n";
                std::cout<<"Error:"<<nm_cmd_dicc<<std::endl;
                std::cout<<"Error:"<<nm_cmd_parse<<std::endl;
                return 0;
            }

           /**
            * Preprocess dicc for repair 
            */
            std::string curr_file = file+"_parse_"+std::to_string(iteration);
            std::string file_dicc = file+"_dicc_"+std::to_string(iteration);
            std::fstream fin(file_dicc,std::ios::in);
            std::fstream fout(file_dicc+"_rr",std::ios::out);

            if(!fin.is_open()){

                std::cout<<"ERROR OPENING "+file_dicc+"\n";
            }
            
            /**This add special symbol between phrases to cheat repair and the return the max of the original symbol*/
            start = std::chrono::steady_clock::now();

            auto n_v = preprocess_dicc(fin,fout,(iteration==0)?1:4);

            duration = std::chrono::duration_cast<std::chrono::microseconds>
                    (std::chrono::steady_clock::now() - start);
            std::cout<<"end preprocess dicc file ... time:"<<duration.count()<<"(microseconds)\n";
            //std::cout<<"preprocess_dicc end...("<<iteration<<") sigma:("<<n_v.first<<","<<n_v.second<<")\n";
            
            /*Remove old file of dicc**/
            
            std::string rm_cmd_old_dicc = "rm "+file_dicc;

                if(system(rm_cmd_old_dicc.c_str()) < 0  ){
                    std::cout<<"Error we can't remove the old dicc file";
                    return 0;
                }
            sigmas.push_back(n_v);
            
            part_file =curr_file;
            ++iteration;

        }
        CLogger::GetLogger()->model["num_it"] = iteration;
       
//       std::cout<<"NUMBER OF ITERATIONS: "<<iteration<<std::endl;


       /* Apply repair to all files file_it_rr */

       for(size_t i = 0; i < iteration; i++)
       {
           std::string c_file = file + "_dicc_"+std::to_string(i)+"_rr";


           auto start = std::chrono::steady_clock::now();
           if(compressor.compress(c_file) < 0 ){
               std::cout<<"Error executing Re-Pair\n";
               return 0;
           }
           auto duration = std::chrono::duration_cast<std::chrono::microseconds>
                   (std::chrono::steady_clock::now() - start);
           CLogger::GetLogger()->model["it_"+std::to_string(i)+"_repair_times"] = duration.count();
           std::cout<<"end apply repair dicc["<<i<<"] file... time:"<<duration.count()<<"(microseconds)\n";

       }

        {

            /*Apply repair to the last parser**/
            std::string c_file = file + "_parse_"+std::to_string(iteration-1);

            auto start = std::chrono::steady_clock::now();
            if(compressor.compress(c_file) < 0 ){
                std::cout<<"Error executing Re-Pair\n";
                return 0;
            }
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>
                    (std::chrono::steady_clock::now() - start);
            CLogger::GetLogger()->model["last_parser_file_repair_times"] = duration.count();
            std::cout<<"end apply repair last parser file... time:"<<duration.count()<<"(microseconds)\n";
        }

        /* *
         * Posprocessing files 
        */
        auto start = std::chrono::steady_clock::now();

        size_t total_size = posprocessing_files(file,iteration,sigmas);
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>
                (std::chrono::steady_clock::now() - start);
        CLogger::GetLogger()->model["posprocessing_time"] = duration.count();
        std::cout<<"end posprocessing files... time:"<<duration.count()<<"(microseconds)\n";
        return total_size;
    }

    std::pair<uint,uint> posprocessing_parse_file(const std::string &file,std::fstream& Gf,std::fstream& Cf,const uint &off_rules, std::unordered_map<uint,uint>&S){
        
        ///std::cout<<"posprocessing_parse_file\n";

            
                /*Updating rules*/
                std::fstream r_file(file+".R",std::ios::in);
        ///        std::cout<<"reading "+file+".R"<<std::endl;
                int maxsigma = 0, n_rules = 0;

            {    
                r_file.read((char*)&maxsigma,4);
                /** jump the characters */
                //r_file.seekg(sigma*sizeof(int),std::ios::beg);
                /**reading rules*/
                
                int X,Y;
                size_t log_n_rules = 0;
                while(!r_file.eof() && r_file.read((char*)&X,4) && r_file.read((char*)&Y,4))
                {
                    /* If X is an element of the alp we change it by the corresponding non-terminal**/
                    if(X < maxsigma)X = S[X];
                    /* If X is a rule we add it the corresponding offset**/
                    else X = X - maxsigma + off_rules;
                    /*Same for Y**/
                    if(Y < maxsigma)Y = S[Y];
                    else Y = Y - maxsigma + off_rules;
                    /*Write in the file*/
                    Gf.write((char*)&X,4);
                    Gf.write((char*)&Y,4);

                    //std::cout<<n_rules+off_rules<<"->"<<X<<" "<<Y<<"\n";
                    /*Increase the number of rules*/
                    ++n_rules;
                    log_n_rules++;
                }
                CLogger::GetLogger()->model["parser_file_posproc_repair_rules"] = log_n_rules;
//                CLogger::GetLogger()->model["repair_rules_last_parse_file"] = log_n_rules;
            }
            
            uint fc = 0;
            {   
                /*Updating first rule*/
                std::fstream c_file(file+".C",std::ios::in);
             //   std::cout<<"reading "+file+".C"<<std::endl;
                int X = 0;
                size_t log_seq = 0;
                while(!c_file.eof() && c_file.read((char*)&X,4))
                {
                    /* If X is an element of the alp we change it by the corresponding non-terminal**/
                    if(X < maxsigma){
                        X = S[X];

                    }
                    /* If X is a rule we add it the corresponding offset**/
                    else{
                        X = X - maxsigma + off_rules;

                    }
                    //std::cout<<X<<" ";
                    Cf.write((char*)&X,4);
                    fc++;
                    log_seq++;
                    
                }
//                CLogger::GetLogger()->model.repair_rules_in_each_it.push_back(log_seq);
//                CLogger::GetLogger()->model["repair_seq_last_parse_file"] = log_seq;
                CLogger::GetLogger()->model["parser_file_posproc_repair_sequence"] = log_seq;
                //std::cout<<std::endl;

            }

            return std::make_pair(n_rules,fc);
            
    }


    size_t decompress(const std::string& file){

        std::fstream out_file(file+"_dec",std::ios::out);
        std::fstream r_file(file+".R",std::ios::in);

        if(!out_file.is_open())
            {
                std::cout<<"Error opening file for decompression\n";
                return 0;
            }
        if(!r_file.is_open())
            {
                std::cout<<"Error opening rules file\n";
                return 0;
            }
        std::unordered_map<uint,std::pair<int,int>> rules;
        std::unordered_map<uint,char> map_sigma;

        int maxsigma = 0;
        r_file.read((char*)&maxsigma,4);
//        std::cout<<"sigma:"<<maxsigma<<std::endl;

        //int maxsigma = 0;

        /* for(size_t i = 0; i < sigma; i++){
            int t = 0;
            r_file.read((char*)&t,4);
            maxsigma = ( maxsigma > t )?maxsigma:t;
            map_sigma[t] =  (char)t;
            //std::cout<<i+1<<":"<<map_sigma[t]<<std::endl;

        } */
        
        uint id = maxsigma;
        int X,Y;
        while(!r_file.eof() && r_file.read((char*)&X,4) && r_file.read((char*)&Y,4)){
            rules[++id] = std::make_pair(X,Y);
//            if(id-maxsigma < 10)
//                std::cout<<id<<"->"<<X<<" "<<Y<<"\n";
        }
            

        //std::cout<<"Number of rules: "<<rules.size();

        std::fstream c_file(file+".C",std::ios::in);
        
        std::function< void (const int& , std::fstream & ) > f;
        
        f = [&maxsigma, &rules, &f](const int& i, std::fstream& out ){
                //std::cout<<i<<std::endl;    
                if(i <= maxsigma ){
                    //char c = map_sigma[i];
                    out.write((char*)&i,1);
                  //  std::cout<<(char)i;
                    return;

                }
                if(rules.find(i) == rules.end()){
                    std::cout<<"Error in the files\n";
                    return;
                }
                //std::cout<<i<<"->"<<rules[i].first<<" "<<rules[i].second<<std::endl;
                f(rules[i].first,out);
                f(rules[i].second,out);                    
        };

        int C = 0;
        uint fcont = 0;
        while(!c_file.eof() && c_file.read((char*)&C,4)){
            f(C,out_file);
            ++fcont;
        }
            
        

        return 2*rules.size()+fcont;
    }




};

#endif //RRPAIR_BIGRREPAIR_H
