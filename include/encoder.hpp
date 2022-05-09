#pragma once

#include <set>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>

#include "huff.hpp"


namespace huff {

    unsigned char code_to_int(std::string* s, int pos);

    class HuffEncoder {
        friend class HuffTester;
    public:
        HuffEncoder(std::istream* input, std::ostream* output);
        ~HuffEncoder();
    
        void count_chars();
        void dict_to_tree();
        void buid_tree();
        void build_codes();

        int encode();
        void restart_input();
        void print_info();

        int get_tree_size();
    
    private:
        void encode_key();
        void write_iteration(std::string accum, size_t max_pos);

    private:
        std::istream* in;
        std::ostream* out;
        size_t buff_size;

    private:
        std::map<char, int> dict;
        HuffTree T;
        std::map<char, std::string>* codes;
        int total;
    };

}

#include "encoder_impl.hpp"