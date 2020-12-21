//
// Created by via56 on 29-11-20.
//


#include "../../include/rr/utils.hpp"





void rr::append(std::vector<uint8_t>& dest,const std::vector<uint8_t>& src, const uint32_t& n){
    for (uint32_t i = 0; i < n ; ++i) {
        dest.push_back(src[i]);
    }
}

void rr::append(std::vector<uint8_t>& dest,const uint8_t* src, const uint32_t& n){
    for (uint32_t i = 0; i < n ; ++i) {
        dest.push_back(src[i]);
    }
}

// power modulo prime
uint64_t rr::modpow(uint64_t base, uint64_t exp,uint64_t prime)
{
    assert(exp>0);
    if(exp==1) return base;
    if(exp%2==0)
        return modpow((base*base)%prime,exp/2,prime);
    else
        return (base*modpow((base*base)%prime,exp/2,prime)) % prime;
}


std::pair<uint64_t,uint64_t> rr::prepareDiccFileForRP(const std::string &dFile, const std::string &dout, int bytes, uint64_t  max_cont) {
    //Open Dicc file
    std::fstream ffile(dFile, std::ios::in|std::ios::binary);
    if (!ffile.is_open()) {

#ifdef RR_DEBUG_PRINT
        std::cout << "Error opening the file: " << dFile << std::endl;
#endif
        throw "Error opening the file:" + dFile;
    }
    //Open output file
    std::fstream fout(dout, std::ios::out|std::ios::binary);
    if (!fout.is_open()) {

#ifdef RR_DEBUG_PRINT
        std::cout << "Error opening the file: " << dout << std::endl;
#endif
        throw "Error opening the file:" + dout;
    }

    rpair_type_write cont = max_cont;
    rpair_type_write c = 0,max = 0;
    //Read Dicc File

#ifdef RR_DEBUG_PRINT
    std::vector<uint_t_read> word;
#endif

    while (!ffile.eof() && ffile.read((char *) &c, bytes))
    {
        if(c > max) max = c;


        if(c == 0){
            cont++;
            fout.write((const char* )& cont, sizeof(rpair_type_write ));


#ifdef RR_DEBUG_PRINT
            std::cout<<"dicc["<<cont<<"]:";
            for (const auto &item : word)
                std::cout<<item<<" ";
            std::cout<<"["<<cont<<"]"<<std::endl;
            word.clear();
#endif

        }else{

#ifdef RR_DEBUG_PRINT
            word.push_back(c);
#endif
            fout.write((const char* )& c, sizeof(rpair_type_write ));
        }

    }

#ifdef RR_DEBUG_PRINT
    std::cout<<"Preparing dicc cont: - <"<<cont<<"-"<<cont<<">"<<std::endl;
    std::cout<<"Preparing dicc max-cont: - <"<<max_cont<<"-"<<cont<<">"<<std::endl;
    std::cout<<"Preparing dicc max-value: - <"<<max_cont<<"-"<<cont<<">"<<std::endl;

#endif

    return std::make_pair(cont,max);
}

uint64_t rr::decompress(const std::string &file) {

#ifdef RR_DEBUG_PRINT
    std::cout<<"DECOMPRESING.............\n";
    std::cout<<file+".R"<<std::endl;
    std::cout<<file+".C"<<std::endl;
#endif
    std::fstream out_file(file+".rrout",std::ios::out);
    std::fstream r_file(file+".R",std::ios::in);

    if(!out_file.is_open())
    {
        std::cout<<"Error opening file for decompression\n";
        throw "Error opening file for decompression\n";
    }
    if(!r_file.is_open())
    {
        std::cout<<"Error opening rules file\n";
        throw "Error opening rules file\n";
    }


    std::unordered_map<uint_t_read ,std::pair<uint_t_read  ,uint_t_read  >> rules;
    std::unordered_map<uint_t_read  ,char> map_sigma;

    uint_t_read   maxsigma = 0;
    r_file.read((char*)&maxsigma,sizeof(uint_t_read  ));


#ifdef RR_DEBUG_PRINT
    std::cout<<"sigma-max:"<<maxsigma<<std::endl;
    uint_t_read  i =1;
#endif

    uint_t_read  id = maxsigma;
    uint_t_read   X = 0,Y = 0;
    while(!r_file.eof() && r_file.read((char*)&X,sizeof(int)) && r_file.read((char*)&Y,sizeof(int))){
        rules[++id] = std::make_pair(X,Y);


#ifdef RR_DEBUG_PRINT

        std::cout<<maxsigma+i<<"-<";
//        if(X <= maxsigma) std::cout<<p1;
//        else
            std::cout<<X;
        std::cout<<",";
//        if(Y <= maxsigma)
//            std::cout<<p2;
//        else
            std::cout<<Y;
        std::cout<<">"<<std::endl;
        ++i;
#endif

    }

    std::fstream c_file(file+".C",std::ios::in);

    std::function< void (const uint_t_read  & , std::fstream & ) > f;
#ifdef RR_DEBUG_PRINT
    std::string cc = "";
#endif
    f = [&maxsigma, &rules, &f
#ifdef RR_DEBUG_PRINT
            ,&cc
#endif
         ](const uint_t_read  & i, std::fstream& out ){
        if(i <= maxsigma ){
            auto ii = char(i);
            std::cout<<ii<<std::endl;
            out.write((const char *)&ii,sizeof(char));

#ifdef RR_DEBUG_PRINT
            cc = cc + char(i);
#endif
            return;
        }
        if(rules.find(i) == rules.end())
            return;

        f(rules[i].first,out);
        f(rules[i].second,out);
    };
    uint_t_read  C = 0;
    uint_t_read  fcont = 0;

#ifdef RR_DEBUG_PRINT
    std::cout<<"S\n";
#endif
    while(!c_file.eof() && c_file.read((char*)&C,sizeof(uint_t_read))){

#ifdef RR_DEBUG_PRINT
        cc = "";
#endif
        f(C,out_file);
        ++fcont;

#ifdef RR_DEBUG_PRINT
        std::cout<<C<<"-"<<C<<std::endl;
#endif

    }

#ifdef RR_DEBUG_PRINT
    std::cout<<std::endl;
#endif

    return 2*rules.size()+fcont;
}
