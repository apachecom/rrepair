//
// Created by via56 on 13-11-20.
//

#include "../../include/lc/parser.hpp"

using namespace lc;


#ifdef PRINT_LOGS
template<>
void lc::print_parser<lc::parser<>>(const lc::parser<>&);
#endif

template<>
void lc::decompress<lc::parser<>>(std::fstream &f, lc::parser<> &parser) {

    std::function<int(lc::parser<>::rule_type *)> dfs;
    dfs = [&dfs, &f, &parser](lc::parser<>::rule_type *X) {
        if (X->type() == MS_SYM) {
            if (X->get_id() != ZERO_SYM) {
                f << parser.alph[X->get_id()];
            }
            return 1;
        }
        if (X->type() == MS_RUN) {
            for (size_t i = 0; i < X->len(); i++)
                dfs(X->first());
            return 1;
        }
        for (size_t i = 0; i < X->len(); i++) {
            dfs((*X)[i]);
        }
        return 1;

    };
    if (parser.S != nullptr)
        dfs(parser.S);
}

template<>
void lc::decompress<lc::parser<>>(std::string &s, lc::parser<> &parser) {

    s.resize(parser.n_text - 2);
    uint64_t off = 0;
    std::function<int(lc::parser<>::rule_type *)> dfs;
    dfs = [&dfs, &s, &off, &parser](lc::parser<>::rule_type *X) {
//        std::cout<<X->get_id()<<std::endl;
        if (X->type() == MS_SYM) {
            if (X->get_id() != ZERO_SYM) {
                s[off] = parser.alph[X->get_id()];
                off++;
            }
            return 1;
        }
        if (X->type() == MS_RUN) {
            for (size_t i = 0; i < X->len(); i++)
                dfs(X->first());
            return 1;
        }
        for (size_t i = 0; i < X->len(); i++) {
            dfs((*X)[i]);
        }
        return 1;

    };
    if (parser.S != nullptr)
        dfs(parser.S);
}

template<>
void
lc::runs<parser<>>(std::vector<typename parser<>::rule_type *> &MT, std::vector<uint64_t> &POS, lc::parser<> &parser) {


    uint32_t off_rule = 0;
    size_t len = MT.size();
    uint64_t total_runs = 1;
    sdsl::bit_vector pos(len, 0);


    for (size_t i = 1; i < len; i++) {
        if (MT[i] != MT[i - 1]) {
            ++total_runs;
            pos[i - 1] = 1;
        }
    }
    pos[len - 1] = 1;

#ifdef PRINT_LOGS
    std::cout<<"RUN-BITVETOR(rank:"<<total_runs<<")"<<std::endl;
    for(size_t i = 0 ; i < len; i++)
        std::cout<<pos[i];
    std::cout<<std::endl;
#endif


    uint64_t c_runs = 0, last_post = 0; //position in the metatext
    std::vector<lc::parser<>::rule_type *> RUNS(total_runs, 0);
    std::vector<uint64_t> POS_RUNS(total_runs, 0);
    std::vector<lc::parser<>::rule_type *> LEVEL_RUNS;

    uint64_t last_off = 0; //position in the text
    for (size_t i = 0; i < len; i++) {
        if (pos[i] == 1) {
            /**
             * search for the run in the hash table
             * */
            lc::parser<>::rule_type *t_run = nullptr;

            if (i - last_post + 1 > 1) {
                // run-len greater than 1
                std::basic_string<char> exp;
                if (!check_expansion(last_off, POS[i] + 1, exp, parser)) {
                    // no existe  la regla
                    t_run = new lc::parser<>::run_type(parser.n_sigma + 1, MT[i], i - last_post + 1);
                    parser.hash_rules[exp] = t_run;
                    LEVEL_RUNS.push_back(t_run);
                    parser.n_sigma++;

                } else {
                    t_run = parser.hash_rules[exp];
                }

                RUNS[c_runs] = t_run;
            } else
                RUNS[c_runs] = MT[i];

            POS_RUNS[c_runs++] = POS[i];
            last_post = i + 1;
            last_off = POS[i] + 1;
        }
    }


#ifdef PRINT_LOGS
    std::cout<<"RUNS-TEXT("<<RUNS.size()<<")"<<std::endl;
    for(size_t i = 0 ; i < RUNS.size(); i++)
        std::cout<<RUNS[i]->get_id()<<" ";
    std::cout<<std::endl;

    std::cout<<"RUNS-POS-TEXT("<<POS_RUNS.size()<<")"<<std::endl;
    for(size_t i = 0 ; i < POS_RUNS.size(); i++)
        std::cout<<POS_RUNS[i]<<" ";
    std::cout<<std::endl;

#endif


    MT.clear();
    POS.clear();
    MT.resize(RUNS.size());
    POS.resize(POS_RUNS.size());
    std::copy(RUNS.begin(), RUNS.end(), MT.begin());
    std::copy(POS_RUNS.begin(), POS_RUNS.end(), POS.begin());
}

template<>
void lc::blocks<lc::parser<>>(std::vector<typename lc::parser<>::rule_type *> &MT, std::vector<uint64_t> &POS,
                              lc::parser<> &parser) {

    uint32_t off_rule = 0;
    size_t len = MT.size();
    uint64_t total_block = 0;
    sdsl::bit_vector pos(len, 0);

    if (len > 2) {
        for (size_t i = 1; i < len - 1; i++) {
            if (MT[i - 1]->get_pi() > MT[i]->get_pi() && MT[i]->get_pi() < MT[i + 1]->get_pi()) {
                ++total_block;
                pos[i] = 1;
            }
        }
    }
    // add the last block
    ++total_block;
    pos[len - 1] = 1;
    //-----------------

#ifdef PRINT_LOGS
    std::cout<<"BLOCK-BITVETOR(rank:"<<total_block<<")"<<std::endl;
    for(size_t i = 0 ; i < len; i++)
        std::cout<<pos[i];
    std::cout<<std::endl;
#endif

    uint64_t c_blocks = 0, last_post = 0, last_off = 0;
    std::vector<lc::parser<>::rule_type *> BLOCKS(total_block, 0);
    std::vector<uint64_t> POS_BLOCKS(total_block, 0);
    std::vector<lc::parser<>::rule_type *> LEVEL_BLOCKS;

    for (size_t i = 0; i < len; i++) {
        if (pos[i] == 1) {
            /**
             * search for the block in the hash table
             * */
            lc::parser<>::rule_type *t_block = nullptr;
            std::string exp;
            if (!check_expansion(last_off, POS[i] + 1, exp, parser)) {
                // new rule
                t_block = new lc::parser<>::block_type(parser.n_sigma + 1, MT.begin() + last_post, MT.begin() + i + 1);
                t_block->exp_size = POS[i] + 1 - last_off;
                parser.hash_rules[exp] = t_block;
                LEVEL_BLOCKS.push_back(t_block);
                parser.n_sigma++;
            } else {
                t_block = parser.hash_rules[exp];
            }

            BLOCKS[c_blocks] = t_block;

            POS_BLOCKS[c_blocks++] = POS[i];

            last_post = i + 1;
            last_off = POS[i] + 1;
        }

    }


#ifdef PRINT_LOGS
    std::cout<<"BLOCKS-TEXT("<<BLOCKS.size()<<")"<<std::endl;
    for(size_t i = 0 ; i < BLOCKS.size(); i++)
        std::cout<<BLOCKS[i]->get_id()<<" ";
    std::cout<<std::endl;

    std::cout<<"BLOCKS-POS-TEXT("<<POS_BLOCKS.size()<<")"<<std::endl;
    for(size_t i = 0 ; i < POS_BLOCKS.size(); i++)
        std::cout<<POS_BLOCKS[i]<<" ";
    std::cout<<std::endl;

#endif

    // if level len is 2 as we add specials simbols it can be equals so no runs and they will be assign 0 and 1 in pi
    if (LEVEL_BLOCKS.size() > 2) {
//    create permutation;
        std::vector<uint64_t> pi;
        lc::permutation(LEVEL_BLOCKS.size() - 2, pi);
        for (uint64_t i = 1; i < LEVEL_BLOCKS.size() - 1; ++i) {
            LEVEL_BLOCKS[i]->set_pi(2 + pi[i - 1]);
        }
    }
    LEVEL_BLOCKS[0]->set_pi(0);
    LEVEL_BLOCKS[LEVEL_BLOCKS.size() - 1]->set_pi(1);

#ifdef PRINT_LOGS
    std::cout<<"PI-BLOCKS("<<BLOCKS.size()<<")"<<std::endl;
    for(size_t i = 0 ; i < BLOCKS.size(); i++)
        std::cout<<BLOCKS[i]->get_pi()<<" ";
    std::cout<<std::endl;
#endif
    MT.clear();
    POS.clear();
    MT.resize(BLOCKS.size());
    POS.resize(POS_BLOCKS.size());
    std::copy(BLOCKS.begin(), BLOCKS.end(), MT.begin());
    std::copy(POS_BLOCKS.begin(), POS_BLOCKS.end(), POS.begin());
}

template<>
void lc::parse_text<lc::parser<>>(const char *_text, const uint64_t &_n_text, lc::parser<> &parser) {

    uint64_t MT_len = _n_text + 2;
    parser.text = new char[MT_len];
    parser.n_text = MT_len;
    std::copy(_text, _text + _n_text, parser.text + 1);

    parser.text[0] = ZERO_SYM;
    parser.text[MT_len - 1] = ZERO_SYM;
    parser.text[MT_len] = 0;

#ifdef PRINT_LOGS
    std::cout<<"NEW-TEXT("<<MT_len<<")"<<std::endl;
    for(size_t i = 0 ; i < MT_len; i++)
        std::cout<<parser.text[i]<<" ";
    std::cout<<std::endl;
#endif
    std::vector<lc::parser<>::rule_type *> MT(MT_len, 0);
    std::map<char, lc::parser<>::rule_type *> sigma;

    for (size_t i = 0; i < MT_len; ++i) {
        lc::parser<>::rule_type *X = nullptr;
        if (sigma.find(parser.text[i]) == sigma.end()) {
            X = new lc::parser<>::rule_type(sigma.size() + 1);
            sigma[parser.text[i]] = X;
        }
        X = sigma[parser.text[i]];
        MT[i] = X;
    }

#ifdef PRINT_LOGS
    std::cout<<"META-TEXT("<<MT.size()<<")"<<std::endl;
    for(size_t i = 0 ; i < MT_len; i++)
        std::cout<<MT[i]->get_id()<<" ";
    std::cout<<std::endl;
#endif

    parser.n_sigma = sigma.size();

    {
        MT[0]->set_pi(0);

        // create first permutation;
        std::vector<uint64_t> pi;
        lc::permutation(sigma.size() - 1, pi);
        uint jj = 0;

        parser.alph.resize(sigma.size());
        for (auto &item : sigma) {
            if (item.first != ZERO_SYM) {
                item.second->set_pi(pi[jj] + 1);
                ++jj;
            }

            parser.alph[item.second->get_id()] = item.first;
        }
        parser.n_pi = sigma.size();


#ifdef PRINT_LOGS
        std::cout<<"PI-META("<<MT.size()<<")"<<std::endl;
        for(size_t i = 0 ; i < MT_len; i++)
            std::cout<<MT[i]->get_pi()<<" ";
        std::cout<<std::endl;
#endif
    }
    std::vector<uint64_t> POS(MT_len, 0);

    for (uint64_t i = 0; i < MT_len; ++i) {
        POS[i] = i;
    }

#ifdef PRINT_LOGS
    std::cout<<"POS-META("<<POS.size()<<")"<<std::endl;
    for(size_t i = 0 ; i < MT_len; i++)
        std::cout<<POS[i]<<" ";
    std::cout<<std::endl;
#endif

    do {
        /**
         * compute the runs
        */

#ifdef PRINT_LOGS
        std::cout<<"incremental-id:"<<parser.n_sigma<<std::endl;
        std::cout<<"incremental-pi:"<<parser.n_pi<<std::endl;
#endif
        runs(MT, POS, parser);
        blocks(MT, POS, parser);
        /**
         * compute blocks
        */
    } while (MT.size() > 1);

    parser.S = MT[0];

}

template<>
void lc::parse_file<lc::parser<>>(const std::string &file, lc::parser<> &Parser) {

    std::fstream Ffile(file, std::ios::in | std::ios::binary);
    uint64_t size_file = io::getFileSize(Ffile);
    char *buffer_text = new char[size_file];
    Ffile.read(buffer_text, size_file);
    Ffile.close();
    lc::parse_text(buffer_text, size_file, Parser);
    delete buffer_text;

}

template<>
bool
lc::check_expansion<lc::parser<>>(const uint64_t &i, const uint64_t &j, std::string &s, const lc::parser<> &parser) {

    s.resize(j - i);
    std::copy(parser.text + i, parser.text + j, s.begin());
    if (parser.hash_rules.find(s) == parser.hash_rules.end())
        return false;
    return true;
}

template<>
void lc::destroy<>(lc::parser<> &parser) {

    std::function<void(lc::parser<>::rule_type *)> f_dfs;
    std::set<lc::parser<>::rule_type *> mark;

    f_dfs = [&f_dfs, &mark](lc::parser<>::rule_type *X) -> void {
        mark.insert(X);
//        std::cout<<X->get_id()<<std::endl;
        switch (X->type()) {
            case MS_SYM: {
                break;
            }
            case MS_RUN: {
                auto Y = X->first();
                if (mark.find(Y) == mark.end())
                    f_dfs(X->first());
                break;
            }
            case MS_BLOCK: {
                for (uint64_t i = 0; i < X->len(); ++i) {
                    auto Y = (*X)[i];
                    if (mark.find(Y) == mark.end())
                        f_dfs(Y);
                }
                break;
            }
        }
        delete X;
        X = nullptr;
    };

    f_dfs(parser.S);
    if (parser.text != nullptr) delete parser.text;

}

#ifdef PRINT_LOGS
template<>
void lc::print_parser<lc::parser<>>(const lc::parser<>& parser){
    std::function< void (lc::parser<>::rule_type* )> f_dfs;
    std::set<uint64_t> mark;
    std::string deep = "";
    f_dfs = [&f_dfs,&mark,&deep](lc::parser<>::rule_type * X) -> void{
        mark.insert(X->get_id());
        std::cout<<deep<<"regla-"<<X->get_id()<<":{\n";
        switch (X->type()) {
            case MS_SYM:{
//                delete X;
                break;
            }
            case MS_RUN:{
                auto Y = X->first();
                deep += "\t";
                if(mark.find(Y->get_id()) == mark.end())
                    f_dfs(X->first());
                deep.pop_back();
                break;
            }
            case MS_BLOCK:{

                deep += "\t";
                for (uint64_t i = 0; i < X->len(); ++i) {
                    auto Y = (*X)[i];
                    if(mark.find(Y->get_id()) == mark.end())
                        f_dfs(Y);
                }
                deep.pop_back();

                break;
            }
        }
        std::cout<<deep<<"}\n";

    };

    f_dfs(parser.S);
}
#endif