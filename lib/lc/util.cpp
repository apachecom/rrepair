//
// Created by via56 on 10-12-20.
//

#include "../../include/lc/util.hpp"

using namespace lc;

void lc::permutation( const uint64_t& len, std::vector<uint64_t> &pi){
    pi.resize(len);
    for(size_t i = 0; i < len; i++)
        pi[i] = i;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(pi.begin(),pi.end(),g);
}
uint_t prepareDiccFileForRP(const std::string &dFile, const std::string &dout, int bytes, uint_t max_value) {
    //Open Dicc file
    std::fstream ffile(dFile, std::ios::in);
    if (!ffile.is_open()) {

#ifdef DEBUG_PRINT
        std::cout << "Error opening the file: " << dFile << std::endl;
#endif
        throw "Error opening the file:" + dFile;
    }
    //Open output file
    std::fstream fout(dout, std::ios::out);
    if (!fout.is_open()) {

#ifdef DEBUG_PRINT
        std::cout << "Error opening the file: " << dout << std::endl;
#endif
        throw "Error opening the file:" + dout;
    }

    uint_t cont = max_value;
    uint_t c = 0;
    //Read Dicc File
    while (!ffile.eof() && ffile.read((char *) &c, bytes))
    {
        if(c == 0){
            cont++;
            fout.write((const char* )& cont, sizeof(uint_t));
        }else{
            fout.write((const char* )& c, sizeof(uint_t));
        }

    }

#ifdef DEBUG_PRINT
    std::cout<<"Preparing dicc: - <"<<max_value<<"-"<<cont<<">"<<std::endl;
#endif

    return cont;
}
uint_t decompress(const std::string& file){

#ifdef DEBUG_PRINT
    std::cout<<"DECOMPRESING.............\n";
#endif
    std::fstream out_file(file+"_dec",std::ios::out);
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
    std::unordered_map<uint_t ,std::pair<uint_t ,uint_t >> rules;
    std::unordered_map<uint_t ,char> map_sigma;

    uint_t  maxsigma = 0;
    r_file.read((char*)&maxsigma,sizeof(uint_t));


#ifdef DEBUG_PRINT
    std::cout<<"sigma-max:"<<maxsigma<<std::endl;
    uint_t i =1;
#endif

    uint_t id = maxsigma;
    uint_t  X,Y;
    while(!r_file.eof() && r_file.read((char*)&X,sizeof(uint_t)) && r_file.read((char*)&Y,sizeof(uint_t))){
        rules[++id] = std::make_pair(X,Y);


#ifdef DEBUG_PRINT
        char p1 = X;
        char p2 = Y;

        std::cout<<maxsigma+i<<"-<";
        if(X <= maxsigma)
            std::cout<<p1;
        else std::cout<<X;
        std::cout<<",";
        if(Y <= maxsigma)
            std::cout<<p2;
        else std::cout<<Y;
        std::cout<<">"<<std::endl;
        ++i;
#endif

    }

    std::fstream c_file(file+".C",std::ios::in);

    std::function< void (const uint_t & , std::fstream & ) > f;

    f = [&maxsigma, &rules, &f](const uint_t & i, std::fstream& out ){
        if(i <= maxsigma ){
            auto ii = char(i);
            std::cout<<ii<<std::endl;
            out.write((const char *)&ii,sizeof(char));

#ifdef DEBUG_PRINT
            cc = cc + char(i);
#endif
            return;
        }
        if(rules.find(i) == rules.end())
            return;

        f(rules[i].first,out);
        f(rules[i].second,out);
    };
    uint_t C = 0;
    uint_t fcont = 0;

#ifdef DEBUG_PRINT
    std::cout<<"S\n";
#endif
    while(!c_file.eof() && c_file.read((char*)&C,sizeof(uint_t ))){

#ifdef DEBUG_PRINT
        cc = "";
#endif
        f(C,out_file);
        ++fcont;

#ifdef DEBUG_PRINT
        std::cout<<C<<"-"<<cc<<std::endl;
#endif

    }

#ifdef DEBUG_PRINT
    std::cout<<std::endl;
#endif

    return 2*rules.size()+fcont;
}
