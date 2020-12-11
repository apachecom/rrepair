//
// Created by via56 on 11-12-20.
//

#include "../../include/rr/confs.hpp"

void rr::mzzConf::init() {

}

bool rr::mzzConf::stopCondition(){
    return (iteration > input.max_iter) || (_size_seq < input.th_initial_seq);
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

//
//    if (isFirstIteration()) {
//        initParserConf();
//    } else {
//        updateParserConf();
//    }
//    //config parse;
//    if (_parser != nullptr)
//        delete _parser;
//
//    _parser = new Parse(_parser_conf);
//    //parse current file
//    _parser->parseFileSM();
//    //posprocess dictionary
//    _size_seq = _parser->results._seq_len;
//    uint_t_64 max_sigma = util::prepareDiccFileForRP(
//            _parser_conf.inputFile() + ".dicc",
//            _parser_conf.inputFile() + "[" + std::to_string(_iter) + "].dicc",
//            sizeof(uint_t_64),
//            _parser->results._max_alph_val
//    );
//
//
//    maxSigmaIt.emplace_back(_parser->results._max_alph_val, max_sigma);
//
//    //store the dicc file for later compression
//    diccFiles.push_back(_parser_conf.inputFile() + "[" + std::to_string(_iter) + "].dicc");
//    //store the parse file, this file will be the next to parse
//    //this files will be removed in the destructor
//    parseFiles.push_back(_parser_conf.inputFile() + ".parse");
//
//    increaseIter();
//

}

void rr::mzzConf::postprocess(){}
