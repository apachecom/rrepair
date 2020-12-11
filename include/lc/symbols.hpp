//
// Created by via56 on 09-11-20.
//

#ifndef LCP_LC_SYMBOL_HPP
#define LCP_LC_SYMBOL_HPP

#include <string>
#include <vector>
#include <list>
#include <set>
#include <iostream>
#include <sdsl/io.hpp>


namespace lc {

    enum TYPE_METASYMBOL{ MS_BLOCK = 'B', MS_RUN = 'R', MS_SYM = 'C'};

    class MetaSymbol;
    class MSrun;
    class MSblock;
/**
 * Class to handle metasymbols of
 *
*/
    class MetaSymbol {

    public:
        typedef std::list<MetaSymbol*>::iterator iter_all;

    protected:
        unsigned int id;

    public:
        unsigned int exp_size;
        uint64_t pi;

        MetaSymbol();


        MetaSymbol(const unsigned int & _c);

        virtual ~MetaSymbol();

        virtual TYPE_METASYMBOL type()const;

        virtual void set_pi(const uint64_t &i){
            pi  = i;
        }
        virtual uint64_t get_pi() const{ return pi;}

        bool is_run(){return type() == MS_RUN;}

        MetaSymbol(const MetaSymbol& MS);
        virtual unsigned int get_symbol();
        unsigned int exp_len() const {return exp_size;}
        uint64_t expansion_len() const {return exp_size;}
        virtual MetaSymbol* first();

        virtual unsigned long size_in_bytes();

        virtual std::string expansion();

        virtual unsigned long size();

        virtual unsigned long len();

        virtual void save(std::fstream& f,std::set<unsigned int>&);

        virtual void load(std::fstream& f,std::unordered_map<unsigned int, MetaSymbol*>& );

        virtual MetaSymbol* operator[](const int & i);
        virtual void remove(const uint & i){};


        virtual void set_vector(std::vector<MetaSymbol*>& V){ }
        virtual std::vector<MetaSymbol*>* get_vector(){ return NULL;}
        virtual std::string toString();
        bool operator == (const MetaSymbol& MS);

        bool operator != (const MetaSymbol& MS);

        bool operator < (const MetaSymbol& MS);

        bool operator > (const MetaSymbol& MS);

        const MetaSymbol& operator = (const MetaSymbol& MS);

        void set_id(const unsigned int& _c);

        unsigned int get_id();
        unsigned int get_label()const { return id;};
        virtual unsigned int children() { return 0; }
        virtual std::string right_hand();

    };



    class MSrun: public MetaSymbol{

    protected:
        MetaSymbol* symbol;
        unsigned long n;
    public:
        /*
         *
         * Default Constructor
         *
         * */
        MSrun();
        ~MSrun();
        /*
         *
         * Copy constructor
         *
         * */
        MSrun(const MSrun& MSr);
        /*
         *
         * Constructor for a run
         *
         * */
        MSrun(const unsigned long & id, MetaSymbol * a, const unsigned int& _n);
        /*
         *
         * Override type method
         *
         * */
        TYPE_METASYMBOL type()const;
        /*
         *
         * Override access method
         *
         * */
        virtual unsigned int get_symbol();

        virtual void set_pi(const uint64_t &i){
            std::cout<<"nothing to do";
            // symbol->set_pi(i);
        }
        virtual uint64_t get_pi() const{ return symbol->get_pi();}

        virtual unsigned long size();

        virtual unsigned long len();

        virtual void save(std::fstream& f,std::set<unsigned int>&);

        void load(std::fstream& f,std::unordered_map<unsigned int, MetaSymbol*>& );

        virtual MetaSymbol* first();

        MetaSymbol* operator[](const int & i);
        /*
         *
         * toString Method give info about the symbol
         *
         * */
        std::string toString();
        /*
         *
         * return the expansion of the symbol
         *
         * */

        std::string expansion();

        const MSrun& operator = (const MSrun& MSr);

        std::string right_hand();

        virtual unsigned int children() { return n > 1? 2:1; }

        void remove(const uint & i){
            delete symbol;
            symbol = NULL;
            n = 0;
        }


    };


    class MSblock:public MetaSymbol{

    protected:

        std::vector<MetaSymbol*> rule;

    public:
        /*
        *
        * Default Constructor
        *
        * */
        MSblock();
        /*
        *
        * Copy Constructor
        *
        * */
        MSblock(const MSblock & MSb);
        /*
        *
        * Constructor with two block positions block[b,e)
        *
        * */
        MSblock(const unsigned long & id, iter_all &b, iter_all &e);

        MSblock(const unsigned long & id, const std::vector<MetaSymbol*>::iterator &b, const std::vector<MetaSymbol*>::iterator &e);

        MSblock(const unsigned long & id, std::vector<MetaSymbol*> & X);

        ~MSblock();
        /*
        *
        * Override type method
        *
        * */
        TYPE_METASYMBOL type() const;
        /*
        *
        * Override access method
        *
        * */
        virtual unsigned long size();
        virtual unsigned long len();
        virtual MetaSymbol* first();



        virtual void save(std::fstream& f,std::set<unsigned int>&);

        void load(std::fstream& f,std::unordered_map<unsigned int, MetaSymbol*>& );

        MetaSymbol* operator[](const int & i);
        /*
        *
        * toString Method give info about the symbol
        *
        * */
        std::string toString();
        /*
        * return the symbol expansion
        *
        * */
        std::string expansion();

        /*
        *
        * = operator
        *
        * */
        const MSblock& operator = (const MSblock& MSb);
        std::string right_hand();

        void set_vector(std::vector<MetaSymbol*>& V);
        std::vector<MetaSymbol*>* get_vector(){ return &rule;}

        virtual unsigned int children() { return rule.size(); }
        void remove(const uint & i){
            auto it = rule.begin() + i;
            delete rule[i];
            rule.erase(it);
        }
    };

}


#endif //LCP_LC_SYMBOL_HPP

