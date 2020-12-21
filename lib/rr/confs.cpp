//
// Created by via56 on 11-12-20.
//

#include <map>
#include "../../include/rr/confs.hpp"


void rr::mzzConf::init(const params * p) {

    input  = *p;
    rr::init(input.w,parser.windows);
    parser.mod = p->m;
//    parseFiles.push_back(input.filename);
}

bool rr::mzzConf::stopCondition(){
    return (iteration > input.max_iter)
    || (_size_seq <= input.th_initial_seq)
    ;
}

bool rr::mzzConf::isFirstIteration(){
    return !iteration;
}

void rr::mzzConf::compressIteration(){

    //apply repair and store repair files
    for (std::string s:diccFiles) {
        if (compressor.compress(s) > 0) {
            repairFilesR.push_back(s + ".R");
            repairFilesC.push_back(s + ".C");
        } else
            throw "[ERROR] RePair compression fail dicc file " + s;

    }
    std::string last_parse_file = parseFiles.back();
    if (compressor.compress(last_parse_file) > 0) {
        repairFilesR.push_back(last_parse_file + ".R");
        repairFilesC.push_back(last_parse_file + ".C");
    } else throw "[ERROR] RePair compression fail last parser file";


}

void rr::mzzConf::parseIteration(){
    bool isFirst  = isFirstIteration();
    if (isFirst) {
        target_file = input.filename;
        parser.windows.bytexsymb = 1;

    } else {

        target_file = parseFiles[iteration - 1];
        parser.windows.bytexsymb = 32;


    }
    //config parse;
    compress(target_file,parser);
    _size_seq = parser.results._seq_len;
    auto p = prepareDiccFileForRP(
            target_file + ".dicc",
            target_file + "[" + std::to_string(iteration) + "].dicc",
            isFirst?1:sizeof(uint_t_read),
            parser.results._max_alph_val
    );


    maxSigmaIt.emplace_back(p);

    //store the dicc file for later compression
    diccFiles.push_back(target_file + "[" + std::to_string(iteration) + "].dicc");
    //store the parse file, this file will be the next to parse
    //this files will be removed in the destructor
    parseFiles.push_back(target_file + ".parse");

    ++iteration;

}

void rr::mzzConf::postprocess(){

    // create a file for the final rules...
    std::fstream R(input.filename + ".R", std::ios::out | std::ios::binary);

    // write the initial sigma value
    uint64_t initial_sigma = maxSigmaIt.front().first;
    R.write((const char *) &initial_sigma, sizeof(uint_t_write));


#ifdef RR_DEBUG_PRINT
    std::cout << "initial sigma value:" << initial_sigma << std::endl;
#endif
    // keep the rules ids.
    std::map<uint64_t, uint64_t> rule_map;
    // last id of the rules assigned
    uint64_t offset_rules = initial_sigma + 1;
    uint64_t total_rules = 0;


    for (uint64_t i = 0; i < repairFilesR.size() - 1; ++i) {
#ifdef RR_DEBUG_PRINT
                std::cout << "Posprocessing files R and C of dicc:" << i + 1 << std::endl;
                std::cout << "Posprocessing R :" << repairFilesR[i] << std::endl;
                std::cout << "Posprocessing C :" << repairFilesC[i] << std::endl;
#endif
        //open files C and R
        std::fstream fileR(repairFilesR[i], std::ios::in | std::ios::binary);
        std::fstream fileC(repairFilesC[i], std::ios::in | std::ios::binary);
        //process file R and return the number of rules
        uint64_t n_rules_level = postprocessDiccRFile(fileR, R, offset_rules, maxSigmaIt[i].first, rule_map);
        //process file C (Create rules for each phrase in the level)
            postprocessDiccCFile(fileC, R, n_rules_level, offset_rules, maxSigmaIt[i].first, maxSigmaIt[i].second,rule_map);
        /* Update the offset of the rules adding the number of rules created in this iteration**/
        offset_rules += n_rules_level;
        total_rules += n_rules_level;
#ifdef RR_DEBUG_PRINT
        std::cout << "*********************************************************************" << std::endl;
#endif
    }

    /**
    *  Process last parser file stored in repairFilesR.last
    * */



    std::fstream C(input.filename + ".C", std::ios::out | std::ios::binary);
    std::pair<uint64_t, uint64_t> p = posprocessingFinalParseFile(R, C, offset_rules, initial_sigma,
                                                                    rule_map);
//    return std::make_pair(total_rules + p.first, p.second);


}

void rr::mzzConf::postprocessDiccCFile(std::fstream &file, std::fstream &R, uint64_t &n_rules_level, const uint64_t &offset_rules, const uint64_t &min_sigma,const uint64_t &max_sigma, std::map<uint64_t, uint64_t> &rule_map) {

    typedef uint64_t uint_t_64;

    /* store the current rule*/
    std::vector<uint_t_64> X;
    /* read the current rule*/
    int _rule = 0;
    uint_t_64 level_rule_id = 0;
    std::map<uint_t_64, uint_t_64> rule_map_level;

#ifdef RR_DEBUG_PRINT
    std::cout << "postprocessDiccCFile " << std::endl;
            std::cout << "postprocessDiccCFile n_rules_level " << n_rules_level << std::endl;
            std::cout << "postprocessDiccCFile offset_rules " << offset_rules << std::endl;
            std::cout << "postprocessDiccCFile min_sigma " << min_sigma << std::endl;
            std::cout << "postprocessDiccCFile max_sigma " << max_sigma << std::endl;
            std::cout << "postprocessDiccCFile ruleMapLen: " << rule_map.size() << std::endl;
//            std::cout<<"Parser sequence:"<<std::endl;
#endif

    while (!file.eof() && file.read((char *) &_rule, sizeof(int))) {
        uint_t_64 rule = _rule;
        // rule is an alph or a repair rule (NOT an artificial symbol!!!)
        if (rule <= min_sigma || rule > max_sigma) {
            /*if it is a valid simbol add it to the current rule*/
            if (rule <= min_sigma) // if it is a alph symbol mapping
            {
                uint_t_64 xx = rule_map.empty() ? rule : rule_map[rule];
                X.push_back(xx);

#ifdef RR_DEBUG_PRINT
                if (xx < 71)
                            std::cout << char(xx) << " ";
                        else
                            std::cout << xx << " ";
#endif

            } else // if it is a new rule compute its real id
            {
                uint_t_64 xx = rule - max_sigma - 1 + offset_rules;
                X.push_back(xx);

#ifdef RR_DEBUG_PRINT
                std::cout << xx << " ";
#endif
            }
        } else {


#ifdef RR_DEBUG_PRINT

            std::cout << std::endl;
#endif
            //create a binary rules for each phrase
            if (X.size() > 1) {
                uint_t_64 last = X.size() - 1, i = 0;
                while (i < X.size() - 1) {
                    if (i == last) {
                        X.push_back(X[last]);
                        last = X.size() - 1;
                        ++i;
                    } else {

                        X.push_back(n_rules_level + offset_rules);

                        if (i + 1 == last)
                            last = X.size() - 1;

                        ++n_rules_level;

                        R.write((char *) &X[i], sizeof(uint_t_64));
                        R.write((char *) &X[i + 1], sizeof(uint_t_64));

#ifdef RR_DEBUG_PRINT
                        std::cout << n_rules_level - 1 + offset_rules << "-<" << X[i] << "," << X[i + 1] << ">"
                                          << std::endl;
#endif
                        i += 2;


                    }


                }
                X.clear();
                rule_map_level[++level_rule_id] = n_rules_level - 1 + offset_rules;

            } else {
                rule_map_level[++level_rule_id] = X.front();
                X.clear();
            }
        }
    }
#ifdef RR_DEBUG_PRINT
    std::cout << "postprocessDiccCFile numero de reglas:" << n_rules_level << std::endl;
#endif
    rule_map.clear();
    rule_map = rule_map_level;

}

uint64_t rr::mzzConf::postprocessDiccRFile(std::fstream &file, std::fstream &R, const uint64_t &offset_rules, const uint64_t &min_sigma, std::map<uint64_t, uint64_t> &rule_map) {

    typedef uint64_t uint_t_64;

    rpair_type_read maxsigma = 0;
    file.read((char *) &maxsigma, sizeof(rpair_type_read));

#ifdef RR_DEBUG_PRINT
    std::cout << "postprocessDiccRFile" << std::endl;
            std::cout << "postprocessDiccRFile offset_rules: " << offset_rules << std::endl;
            std::cout << "postprocessDiccRFile min_sigma: " << min_sigma << std::endl;
            std::cout << "postprocessDiccRFile maxsigma: " << maxsigma << std::endl;
            std::cout << "postprocessDiccRFile ruleMapLen: " << rule_map.size() << std::endl;
#endif
    int XX, YY;
    uint_t_64 c = 0;
    // reading repair pairs size(int)
    while (!file.eof() && file.read((char *) &XX, sizeof(rpair_type_read)) && file.read((char *) &YY, sizeof(rpair_type_read))) {
        uint_t_write X = XX, Y = YY;
#ifdef RR_DEBUG_PRINT
        std::cout << XX << "-" << YY << std::endl;
#endif

        /**
         * If X is less than min_sigma then is an element of the initial alph
         * */
//                if(X > min_sigma){
        /* If X is an element of the alp we change it by the corresponding non-terminal**/
        if (X < maxsigma)
            X = rule_map.empty() ? X : rule_map[X];
        else {
            X = (X - maxsigma) + offset_rules;
        }
//                }
        /**
         * Same for Y
         * */
//                if(Y > min_sigma){
        /* If X is an element of the alp we change it by the corresponding non-terminal**/
        if (Y < maxsigma)
            Y = rule_map.empty() ? Y : rule_map[Y];
        else
            Y = (Y - maxsigma) + offset_rules;
//                }

        /*Write in the file*/
        R.write((char *) &X, sizeof(uint_t_write));
        R.write((char *) &Y, sizeof(uint_t_write));

        c++;
    }
#ifdef RR_DEBUG_PRINT
    std::cout << "postprocessDiccRFile number of rules:" << c << std::endl;
#endif

    return c;

}

std::pair<uint64_t, uint64_t> rr::mzzConf::posprocessingFinalParseFile(std::fstream &R, std::fstream &C, uint64_t &offset_rules, const uint64_t &initial_sigma, std::map<uint64_t, uint64_t> &rule_map) {



    /*Updating rules*/
    std::fstream r_file(repairFilesR.back(), std::ios::in);
    rpair_type_read maxsigma = 0, n_rules = 0;

    {
        r_file.read((char *) &maxsigma, sizeof(rpair_type_read));

#ifdef RR_DEBUG_PRINT
        std::cout << "posprocessingFinalParseFile" << std::endl;
                std::cout << "posprocessingFinalParseFile file: " << repairFilesR.back() << std::endl;
                std::cout << "posprocessingFinalParseFile offset_rules " << offset_rules << std::endl;
                std::cout << "posprocessingFinalParseFile initial_sigma " << initial_sigma << std::endl;
                std::cout << "posprocessingFinalParseFile max_sigma " << maxsigma << std::endl;
#endif
        /**reading rules*/
        int XX = 0, YY = 0;
        while (!r_file.eof() && r_file.read((char *) &XX, sizeof(rpair_type_read)) &&
               r_file.read((char *) &YY, sizeof(int))) {
            auto X = (uint_t_write) XX, Y = (uint_t_write) YY;

            /* If X is an element of the alp we change it by the corresponding non-terminal**/
            if (X < maxsigma)X = rule_map[X];
                /* If X is a rule we add it the corresponding offset**/
            else X = X - maxsigma + offset_rules;
            /*Same for Y**/
            if (Y < maxsigma)Y = rule_map[Y];
            else Y = Y - maxsigma + offset_rules;
            /*Write in the file*/
            R.write((char *) &X, sizeof(uint_t_write));
            R.write((char *) &Y, sizeof(uint_t_write));
            /*Increase the number of rules*/

#ifdef RR_DEBUG_PRINT
            char p1 = X;
                    char p2 = Y;
                    std::cout << offset_rules + n_rules + 1 << "-<";
                    if (X <= initial_sigma)
                        std::cout << p1;
                    else std::cout << X;
                    std::cout << ",";
                    if (Y <= initial_sigma)
                        std::cout << p2;
                    else std::cout << Y;
                    std::cout << ">" << std::endl;
#endif

            ++n_rules;
            XX = 0;
            YY = 0;
        }
    }

    uint_t_write len = 0;
    {
        /*Updating first rule*/
        std::fstream c_file(repairFilesC.back(), std::ios::in);
        rpair_type_read XX = 0;

#ifdef RR_DEBUG_PRINT
        std::cout << "FINAL SEQ C" << std::endl;
                std::cout << "posprocessingFinalParseCFile file: " << repairFilesC.back() << std::endl;
#endif
        while (!c_file.eof() && c_file.read((char *) &XX, sizeof(rpair_type_read))) {
            uint_t_write X = XX;
//            if (rule_map.find(X) == rule_map.end())
//                std::cout << "rule doesn't found\n" << X << std::endl;
            if (!X) continue;
            /* If X is an element of the alp we change it by the corresponding non-terminal**/
            if (X < maxsigma) X = rule_map[X];
                /* If X is a rule we add it the corresponding offset**/
            else X = X - maxsigma + offset_rules;
            C.write((char *) &X, sizeof(uint_t_write));

#ifdef RR_DEBUG_PRINT
            if (X < initial_sigma) std::cout << char(X) << " ";
                    else std::cout << X << " ";
#endif
            len++;
            XX = 0;
        }


#ifdef RR_DEBUG_PRINT
        std::cout << std::endl;
#endif
    }

    return std::make_pair(n_rules, len);
}
