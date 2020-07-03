//
// Created by ale on 03-07-20.
//

#include <vector>
#include "RepairUtils.h"

using namespace big_repair;

uint32_t util::prepareDiccFileForRP(const std::string &dFile, const std::string &dout, int bytes, uint32_t max_value) {
    //Open Dicc file
    std::fstream ffile(dFile, std::ios::in);
    if (!ffile.is_open()) {
        std::cout << "Error opening the file: " << dFile << std::endl;
        return 0;
    }
    //Open output file
    std::fstream fout(dout, std::ios::out);
    if (!fout.is_open()) {
        std::cout << "Error opening the file: " << dout << std::endl;
        return 0;
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
