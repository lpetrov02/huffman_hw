#pragma once

#include <set>
#include <map>
#include <utility>
#include <iostream>
#include <fstream>

#include "huff.hpp"


namespace huff {

    std::string reverse(std::string s);
    std::string byte_bin(unsigned char c);

    class HuffDecoder {
        friend class HuffTester;
    public:
        HuffDecoder(std::istream* input, std::ostream* output);

        void decode();

        void get_key_make_tree();
        void build_tree();
        void print_info();

    private:
        std::string delete_end_zeros(std::string s);
        HuffNode* decode_piece(HuffNode* current_node, std::string s, bool special_case);
        int find_input_length();

    private:
        std::istream* in;
        std::ostream* out;
        HuffTree T; 
        int key_length;
    };

}

#include "decoder_impl.hpp"