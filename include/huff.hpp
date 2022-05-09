#pragma once

#include <set>
#include <map>
#include <utility>
#include <iostream>
#include <fstream>

#define UNUSED(x) (void)(x)

namespace huff {    

    class HuffNode {
        friend class HuffTree;
        friend class HuffDecoder;
        friend class HuffEncoder;
        friend class HuffComparator;
        friend class HuffTester;
    public:
        HuffNode(std::string str, float p, HuffNode* r, HuffNode* l);
        HuffNode(char c, float p);
        ~HuffNode();

        bool operator<(const HuffNode& other) const;
        void print(int depth) const;
        void build_codes(std::map<char, std::string>* codes, std::string s) const;

    private:
        std::string s;
        float prob;
        HuffNode* right;
        HuffNode* left;
    };


    struct HuffComparator {
        bool operator()( const HuffNode* a, const HuffNode* b ) const {
            return a->s < b->s;
        }
    };


    class HuffTree {
        friend class HuffDecoder;
        friend class HuffEncoder;
        friend class HuffTester;
    public:
        HuffTree();
        ~HuffTree();

        void print() const;
        int size() const;
        void add(HuffNode* node);
        void unite(HuffNode* first, HuffNode* second);
        std::pair<HuffNode*, HuffNode*> find_two_min() const;
        void build_codes(std::map<char, std::string>* codes) const;

    private:
        std::set<HuffNode*, HuffComparator> nodes;
    };

}


#include "huff_impl.hpp"