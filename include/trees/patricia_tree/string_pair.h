//
// Created by via56 on 23-09-20.
//

#ifndef LCP_STRING_PAIR_H
#define LCP_STRING_PAIR_H


#include <utility>
#include <cstdint>
#include <string>

namespace util{

    typedef  uint64_t text_size;
    typedef  uint64_t len_size;
    class string_pairs:std::pair<text_size ,text_size  > {

        public:
            string_pairs();
            ~string_pairs() = default;
            string_pairs(const std::string& , const len_size& );
            string_pairs(const string_pairs& );
            string_pairs& operator=(const string_pairs& );
            len_size get_id() const;
            void set_left(const text_size &t);
            void set_right(const text_size &t);
            text_size get_left() const;
            text_size  get_right() const;
            void operator++();
            text_size size() const;
            bool empty();
            ulong to_ulong() const;
            virtual char operator[](const text_size &i) const = 0;
            virtual std::string toString() const = 0;




        protected:

            const std::string* text{};
            len_size id{};
            len_size c{};
    };


    class string_pairs_prefix : public string_pairs {

        public:
            string_pairs_prefix(const std::string&T , const len_size&i ):string_pairs(T,i){};
            char operator[](const text_size &i) const override;
            std::string toString() const override;
            const char* begin(){
                return text->c_str() + get_left();
            }
    };

    class string_pairs_suffix : public string_pairs {

        public:
            string_pairs_suffix(const std::string&T , const len_size&i ):string_pairs(T,i){};
            char operator[](const text_size &i) const override ;
            std::string toString() const override;
    };

}


#endif //LCP_STRING_PAIR_H
