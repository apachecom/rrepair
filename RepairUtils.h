//
// Created by ale on 02-07-20.
//

#ifndef RRPAIR_REPAIRUTILS_H
#define RRPAIR_REPAIRUTILS_H

#include <fstream>
#include <iostream>
#include <cstring>

namespace big_repair{

    namespace util{

        /**
         * Read Dicctionary File and prepare the file to apppy Re-Pair
         * Dicctionary File Format s_1,0,s_2,0,.....,s_n,0
         *
         * Output s_1,|A|+1,s_2,|A|+2,.....,s_n,|A|+ n
         * where |A| is the max value in the alph dictionary
         ***/
        void prepareDiccFileForRP(const std::string &dFile, const std::string &dout, int bytes, uint32_t max_value = 0) {
            //Open Dicc file
            std::fstream ffile(dFile, std::ios::in);
            if (!ffile.is_open()) {
                std::cout << "Error opening the file: " << dFile << std::endl;
                return;
            }
            //Open output file
            std::fstream fout(dout, std::ios::out);
            if (!fout.is_open()) {
                std::cout << "Error opening the file: " << dout << std::endl;
                return;
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
        }


        /*
         * Take a sequence S of variables and create a dummy binary grammar
         * for S
         * symbols is the start id to name the new rules
         * Write in output_file
         * */
        void create_binary_grammars(const std::vector<int>& R,std::ofstream& fr_out, uint32_t & symbols)
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


    }

}



#endif //RRPAIR_REPAIRUTILS_H
