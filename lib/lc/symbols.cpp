

//
// Created by alejandro on 11-09-19.
//

#include <set>
#include "../../include/lc/symbols.hpp"

using namespace lc;

MetaSymbol::MetaSymbol()
{
    exp_size = 0;
}

MetaSymbol::MetaSymbol(const MetaSymbol& MS){

    id = MS.id;
    exp_size = MS.exp_size;
}

MetaSymbol::MetaSymbol(const unsigned int & _c)
{
    id = _c;
    exp_size = 1;
}

MetaSymbol::~MetaSymbol()
{
    pi = 0;
}

TYPE_METASYMBOL MetaSymbol::type()const
{
    return MS_SYM;
}

unsigned int MetaSymbol::get_symbol()
{
    return id;
}

MetaSymbol* MetaSymbol::first()
{
    return NULL;
}

unsigned long MetaSymbol::size_in_bytes()
{
    return sizeof(unsigned long);
}

std::string MetaSymbol::expansion()
{
    return std::to_string((char)id)+" ";
}

unsigned long MetaSymbol::size()
{
    return 1;
}
unsigned long MetaSymbol::len()
{
    return 1;
}

void MetaSymbol::save(std::fstream& f, std::set<unsigned int >& M)
{
    M.insert(id);
    char t = type();
    f.write(&t,1);

    f.write((const char *)&id,sizeof(unsigned int));
#ifdef PRINT
    std::cout<<"f << type:char("<<t<<")"<<std::endl;
        std::cout<<"f << id:"<<id<<std::endl;
        std::cout<<"------------------------------------------------"<<std::endl;
#endif

}
void MetaSymbol::load(std::fstream& f,std::unordered_map<unsigned int, MetaSymbol*>& M)
{
    sdsl::load(id,f);
}

MetaSymbol* MetaSymbol::operator[](const int & i)
{
    return NULL;
}

std::string MetaSymbol::toString()
{
    return "symbol:"+std::to_string(id)+" "+ "type:"+ (char)type() + "\n";
}

bool MetaSymbol::operator == (const MetaSymbol& MS)
{
    return id == MS.id;
}

bool MetaSymbol::operator != (const MetaSymbol& MS)
{
    return !(id == MS.id);
}
bool MetaSymbol::operator < (const MetaSymbol& MS)
{
    return id < MS.id;
}

bool MetaSymbol::operator > (const MetaSymbol& MS)
{
    return id > MS.id;
}

const MetaSymbol& MetaSymbol::operator = (const MetaSymbol& MS)
{
    id = MS.id;
    exp_size = MS.exp_size;
    return *this;
}

void MetaSymbol::set_id(const unsigned int& _c)
{
    id = _c;
}

unsigned int MetaSymbol::get_id()
{
    return id;
}

std::string MetaSymbol::right_hand()
{
    return std::to_string(id);
}


/**
 * Class MSrun
 *
 * **/
/*
 *
 * Default Constructor
 *
 * */
MSrun::MSrun():MetaSymbol(){
    symbol = NULL;
    n = 0;
    exp_size = 0;
};
MSrun::~MSrun()
{
}
/*
 *
 * Copy constructor
 *
 * */
MSrun::MSrun(const MSrun& MSr):MetaSymbol(MSr){
    symbol = MSr.symbol;
    n = MSr.n;
}
/*
 *
 * Constructor for a run
 *
 * */
MSrun::MSrun(const unsigned long & id, MetaSymbol * a, const unsigned int& _n):MetaSymbol(id)
{
    symbol = a;
    n = _n;
    exp_size = n*symbol->exp_size;
}
/*
 *
 * Override type method
 *
 * */
TYPE_METASYMBOL MSrun::type()const{ return MS_RUN;}
/*
 *
 * Override access method
 *
 * */

unsigned int MSrun::get_symbol(){ return symbol->get_id();}

unsigned long MSrun::size(){ return 2;}

unsigned long MSrun::len(){ return n;}

void MSrun::save(std::fstream& f,std::set<unsigned int>& M)
{
//        std::cout<<"saving MSrun: "<<id<<" "<<type()<<std::endl;
    M.insert(id);

    char t = type();
//        f << t <<" "<<id<<" "<<n<<" "<<symbol->get_id()<<"\n";

    f.write(&t,1);
    f.write((const char *)&id,sizeof(unsigned int));
    f.write((const char *)&n,sizeof(unsigned long));
    unsigned int id_s = symbol->get_id();
    f.write((const char *)&id_s,sizeof(unsigned int));

#ifdef PRINT
    std::cout<<"f << type:"<<"run("<<t<<")"<<std::endl;
        std::cout<<"f << id:"<<id<<std::endl;
        std::cout<<"f << rule-size:"<<n<<std::endl;
        std::cout<<"f << symbol-id:"<<id_s<<std::endl;
        std::cout<<"------------------------------------------------"<<std::endl;
#endif
    if(M.find(symbol->get_id())== M.end())
        symbol->save(f,M);
}

void MSrun::load(std::fstream& f,std::unordered_map<unsigned int, MetaSymbol*>& M)
{
    f.read((char*)&id,sizeof(unsigned int));
    f.read((char*)&n,sizeof(unsigned long));
    unsigned int id_s = 0;
    f.read(( char *)&id_s,sizeof(unsigned int));
#ifdef PRINT
    std::cout<<"f << type:"<<"run"<<std::endl;
        std::cout<<"f << id:"<<id<<std::endl;
        std::cout<<"f << rule-size:"<<n<<std::endl;
        std::cout<<"f << symbol-id:"<<id_s<<std::endl;
        std::cout<<"------------------------------------------------"<<std::endl;

#endif
    if(M.find(id_s) != M.end()){
        symbol = M[id_s];
    }else{

        char symbol_type = 0;
        f.read(&symbol_type,1);

        switch (symbol_type)
        {
            case MS_BLOCK:
                /* code */
                symbol = new MSblock();
                break;
            case MS_RUN:
                /* code */
                symbol = new MSrun();
                break;
            case MS_SYM:
                /* code */
                symbol = new MetaSymbol();
                break;
            default:
                std::cout<<"Error reading type\n";
                break;
        }

        M[id_s] = symbol;
        symbol->load(f,M);
    }
}

MetaSymbol* MSrun::first(){ return symbol;}

MetaSymbol* MSrun::operator[](const int & i)
{
    //// we have to decide what to do with access to invalid position.
    return symbol;
}
/*
 *
 * toString Method give info about the symbol
 *
 * */
std::string MSrun::toString(){

    return MetaSymbol::toString() + "\n\t run:"+std::to_string(n)+" "+"meta:["+std::to_string(symbol->get_id())+"]\n";
}
/*
 *
 * return the expansion of the symbol
 *
 * */

std::string MSrun::expansion(){

    std::string s = "";
    for (int i = 0; i < n ; ++i) {
        s+=symbol->expansion();
    }
    return s;
}

const MSrun& MSrun::operator = (const MSrun& MSr){

    set_id(MSr.id);
    symbol = MSr.symbol;
    n = MSr.n;
    exp_size = MSr.exp_size;
    return *this;
}

std::string MSrun::right_hand(){
    return std::to_string(symbol->get_id()) + "^{" + std::to_string(n)+"} ";
}




/**
 *
 * Methods of MSblock Class
 *
 *
*/


/*
        *
        * Default Constructor
        *
        * */
MSblock::MSblock():MetaSymbol()
{

};
/*
*
* Copy Constructor
*
* */
MSblock::MSblock(const MSblock & MSb):MetaSymbol(MSb)
{
    rule.resize(MSb.rule.size());
    std::copy(MSb.rule.begin(),MSb.rule.end(),rule.begin());
}
/*
*
* Constructor with two block positions block[b,e)
*
* */

MSblock::MSblock(const unsigned long & id, iter_all &b, iter_all &e)
{
    this->id = id;
    auto t = b;
    exp_size = 0;
    while(t != e){
        rule.push_back(*t);
        exp_size+= (*t)->exp_size;
        ++t;
    }
}
MSblock::MSblock(const unsigned long & id, const std::vector<MetaSymbol*>::iterator &b, const std::vector<MetaSymbol*>::iterator &e)
{
    this->id = id;
    uint32_t len = e - b;
    rule.resize(len);
    std::copy(b, e,rule.begin());

    exp_size = 0;
//            std::cout<<"id:"<<this->get_id()<<std::endl;

    for(auto &&r:rule){
        exp_size += r->exp_size;
//                std::cout<<"\tid:"<<r->get_id()<<std::endl;
//                std::cout<<"\tlen:"<<r->exp_size<<std::endl;
    }
}

MSblock::MSblock(const unsigned long & id, std::vector<MetaSymbol*> & X)
{
    this->id = id;
    rule.resize(X.size());
    std::copy(X.begin(), X.end(),rule.begin());
    exp_size = 0;
//            std::cout<<"id:"<<this->get_id()<<std::endl;

    for(auto &&r:rule){
        exp_size += r->exp_size;
//                std::cout<<"\tid:"<<r->get_id()<<std::endl;
//                std::cout<<"\tlen:"<<r->exp_size<<std::endl;
    }
//            std::cout<<"len:"<<this->exp_size<<std::endl;


}

MSblock::~MSblock()
{

};
/*
*
* Override type method
*
* */
TYPE_METASYMBOL MSblock::type() const
{
    return MS_BLOCK;
}
/*
*
* Override access method
*
* */
unsigned long MSblock::size(){ return rule.size();}
unsigned long MSblock::len(){ return rule.size();}
MetaSymbol* MSblock::first(){ return (rule.empty())?NULL:rule[0];}



void MSblock::save(std::fstream& f, std::set<unsigned int >& M){

    M.insert(id);

    char t = type();

    f.write(&t,1);
    f.write((const char *)&id,sizeof(unsigned int));
    size_t nn = rule.size();
    f.write((const char *)&nn,sizeof(size_t));
#ifdef PRINT
    std::cout<<"f << type:"<<"block("<<t<<")"<<std::endl;
            std::cout<<"f << id:"<<id<<std::endl;
            std::cout<<"f << rule-size:"<<nn<<std::endl;
            std::cout<<"f << rule:[";
#endif
    for (int i = 0; i < rule.size(); ++i) {
        unsigned int id_s = rule[i]->get_id();
        f.write((const char *)&id_s,sizeof(unsigned int));
#ifdef PRINT

        std::cout<<" "<<id_s<<"|";
#endif
    }

#ifdef PRINT

    std::cout<<"]"<<std::endl;
            std::cout<<"------------------------------------------------"<<std::endl;
#endif

    for (int i = 0; i < rule.size(); ++i) {
        if(M.find(rule[i]->get_id())== M.end())
            rule[i]->save(f,M);
    }
}

void MSblock::load(std::fstream& f,std::unordered_map<unsigned int, MetaSymbol*>& M){

    /**we arleady read the type */
    size_t nn = 0;
    sdsl::load(id,f);
    sdsl::load(nn,f);
    rule.resize(nn);
    std::vector<unsigned int> ids(nn);
#ifdef PRINT
    std::cout<<"f << type:"<<"block"<<std::endl;
            std::cout<<"f << id:"<<id<<std::endl;
            std::cout<<"f << rule-size:"<<nn<<std::endl;
            std::cout<<"f << rule:[";
#endif
    for (size_t ii = 0; ii < nn; ii++)
    {
        unsigned int _id = 0;
        f.read((char*)&_id,sizeof(unsigned int));
        ids[ii] = _id;
#ifdef PRINT
        std::cout<<" "<<_id<<"|";
#endif
    }
#ifdef PRINT
    std::cout<<"]"<<std::endl;
#endif
    for (int i = 0; i < nn; ++i) {

        if(M.find(ids[i]) != M.end()){
            rule[i] = M[ids[i]];
        }else{

            char t = 0;
            f.read(&t,1);
            switch (t)
            {
                case MS_BLOCK:
                    /* code */
                    rule[i] = new MSblock();
                    break;

                case MS_RUN:
                    /* code */
                    rule[i] = new MSrun();
                    break;

                case MS_SYM:
                    rule[i] = new MetaSymbol();
                    break;
                default:
                    std::cout<<"Error reading type\n";
                    break;
            }

            M[ids[i]] = rule[i];
            rule[i]->load(f,M);
        }
    }
}

MetaSymbol* MSblock::operator[](const int & i)
{
    //// we have to decide what to do with access to invalid position.
    return rule[i];
}
/*
*
* toString Method give info about the symbol
*
* */
std::string MSblock::toString(){

    std::string s = MetaSymbol::toString() + " \n\tright_hand:";
    for (int i = 0; i < rule.size(); ++i) {
        s += std::to_string(rule[i]->get_id()) + " ";
    }
    return s;

}
/*
* return the symbol expansion
*
* */
std::string MSblock::expansion(){
    //
    //
    std::string s = "";
    for (int i = 0; i < rule.size(); ++i) {
        s += rule[i]->expansion();
    }
    return s;
}

/*
*
* = operator
*
* */

const MSblock& MSblock::operator = (const MSblock& MSb)
{
    set_id(MSb.id);
    rule.clear();
    rule.resize(MSb.rule.size());
    std::copy(MSb.rule.begin(),MSb.rule.end(),rule.begin());
    exp_size = MSb.exp_size;
    return *this;

}


std::string MSblock::right_hand(){
    std::string s = "";
    for (const auto &item : rule) {
        s += std::to_string(item->get_id()) + " ";
    }
    return s;
}

void MSblock::set_vector(std::vector<MetaSymbol*>& V)
{
    this->rule.clear();
    this->rule.resize(V.size());
    std::copy(V.begin(),V.end(),this->rule.begin());

}


