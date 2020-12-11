//
// Created by via56 on 23-09-20.
//

#ifndef LCP_NODE_H
#define LCP_NODE_H

#include <map>
#include <vector>
#include <sdsl/io.hpp>

namespace trees {

    struct node {

        typedef unsigned int ulong;
        typedef std::pair<ulong, node *> edge;

        ulong id;
        ulong ptr_leaf;
        bool leaf;
        char ch{};

        std::multimap<unsigned char, edge> childs;
        std::vector<ulong> ids;

        node(const ulong &id, const ulong &leaf, bool l = false, ulong m = 1) : id(id), ptr_leaf(leaf), leaf(l) {
            childs.clear();
            ids.clear();
            ids.emplace_back(id);
        }
        ~node() {
            for (auto &&item : childs) {
                delete item.second.second;
            }
        }
        ulong size_in_bytes()const {
            return sizeof(ulong) * 2 + childs.size() * 2 + ids.size() + 2;
        }
        void save(std::fstream& f)const{

            sdsl::serialize( id,f);
            sdsl::serialize( ptr_leaf,f);
            sdsl::serialize(leaf,f);
            sdsl::serialize(ch,f);

            sdsl::serialize(childs.size(),f);
            for (auto &&item : childs) {
                sdsl::serialize(item.first,f);
                sdsl::serialize(item.second.first,f);
                sdsl::serialize(item.second.second,f);
            }

            for (auto &&item : ids) {
                sdsl::serialize(item,f);

            }
        }

    };

}

#endif //LCP_NODE_H
