//
// Created by ale on 03-07-20.
//

#include <vector>
#include <unordered_map>
#include <functional>
#include "RepairUtils.h"

using namespace big_repair;
//#define DEBUG_PRINT


#ifdef DEBUG_PRINT
std::string cc = "";
#endif

uint32_t util::prepareDiccFileForRP(const std::string &dFile, const std::string &dout, int bytes, uint32_t max_value) {
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

    uint32_t cont = max_value;
    uint32_t c = 0;
    //Read Dicc File
    while (!ffile.eof() && ffile.read((char *) &c, bytes))
    {
        if(c == 0){
            cont++;
            fout.write((const char* )& cont, sizeof(uint32_t));
        }else{
            fout.write((const char* )& c, sizeof(uint32_t));
        }

    }

#ifdef DEBUG_PRINT
    std::cout<<"Preparing dicc: - <"<<max_value<<"-"<<cont<<">"<<std::endl;
#endif

    return cont;
}


void util::create_binary_grammars(const std::vector<int>& R,std::ofstream& fr_out, uint32_t & symbols)
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


uint32_t util::decompress(const std::string& file){

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
    std::unordered_map<uint32_t ,std::pair<uint32_t ,uint32_t >> rules;
    std::unordered_map<uint32_t ,char> map_sigma;

    uint32_t  maxsigma = 0;
    r_file.read((char*)&maxsigma,4);


#ifdef DEBUG_PRINT
    std::cout<<"sigma-max:"<<maxsigma<<std::endl;
#endif
    uint id = maxsigma;
    uint32_t  X,Y;

#ifdef DEBUG_PRINT
    uint32_t i =1;
#endif

    while(!r_file.eof() && r_file.read((char*)&X,sizeof(uint32_t )) && r_file.read((char*)&Y,sizeof(uint32_t ))){
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

    std::function< void (const uint32_t & , std::fstream & ) > f;

    f = [&maxsigma, &rules, &f](const uint32_t & i, std::fstream& out ){
        if(i <= maxsigma ){
            out.write((char*)&i,1);

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
    uint32_t C = 0;
    uint32_t fcont = 0;

#ifdef DEBUG_PRINT
    std::cout<<"S\n";
#endif

    while(!c_file.eof() && c_file.read((char*)&C,sizeof(uint32_t ))){

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