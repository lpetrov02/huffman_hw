#pragma once

#include "decoder.hpp"
#include "exceptions.hpp"


std::string huff::reverse(std::string s) {
    std::string res;
    auto it = s.rbegin();
    while (it != s.rend()) {
        res.push_back(*it++);
    }
    return res;
}


std::string huff::byte_bin(unsigned char c) {
    std::string s;
    for (int i = 0; i < 8; i++) {
        int bit = c % 2;
        if (bit == 0)
            s.push_back('0');
        else
            s.push_back('1');
        c /= 2;
    }
    return reverse(s);
}


// HuffDecoder

huff::HuffDecoder::HuffDecoder(std::istream* input, std::ostream* output) {
    in = input;
    out = output;

    T = HuffTree();
    key_length = 0;
}


void huff::HuffDecoder::get_key_make_tree() {
    char c;
    float prob;

    in->read((char*)&key_length, 4);
    if (in->bad()) {
        throw FileIOException("reading");
    }

    for (int i = 0; i < key_length; i++) {
        in->read(&c, 1);
        if (in->bad()) {
            throw FileIOException("reading");
        }
        in->read((char*)&prob, 4);
        if (in->bad()) {
            throw FileIOException("reading");
        }
        try {
            T.add(new HuffNode(c, prob));
        } catch (MemoryException& e) {
            throw e;
        }  
    }
}

void huff::HuffDecoder::build_tree() {
    while (T.size() > 1) {
        std::pair<HuffNode*, HuffNode*> p = T.find_two_min();

        try {
            T.unite(p.first, p.second);
        } catch (MemoryException& e) {
            throw e;
        }
    }
}

int huff::HuffDecoder::find_input_length() {
    int pos = in->tellg();
    in->seekg(0, in->end);
    int end_pos = in->tellg();
    in->seekg(pos, in->beg);

    return end_pos;
}

huff::HuffNode* huff::HuffDecoder::decode_piece(huff::HuffNode* current_node, std::string s, bool special_case) {
    auto it = s.begin();
    while (it != s.end()) {
        while(current_node->s.length() > 1 && it != s.end()) {
            if (*it == '1')
                current_node = current_node->right;
            else
                current_node = current_node->left;
            ++it;
        }
        if (current_node->s.length() == 1) {
            out->write(&(current_node->s[0]), 1);
            if (out->bad()) {
                throw FileIOException("writing");
            }

            current_node = *(T.nodes.begin());
            if (special_case)
                ++it;
        }
    }

    return current_node;
}

std::string huff::HuffDecoder::delete_end_zeros(std::string s) {
    int l = s.length();
    int end_zeros = (s[l - 1] - 48) + 2 * (s[l - 2] - 48) + 4 * (s[l - 3] - 48);
    for (int i = 0; i < 8 + end_zeros; i++)
        s.pop_back();
    
    return s;
}

void huff::HuffDecoder::decode() {
    if (T.size() == 0)
        return;

    int stop_pos = find_input_length() - 2;

    char c;
    std::string s;
    huff::HuffNode* current_node = *(T.nodes.begin());
    bool special_case = current_node->s.length() == 1;

    while (in->tellg() < stop_pos && in->read(&c, 1)) {
        if (in->bad()) {
            throw FileIOException("reading");
        }
        s = byte_bin((unsigned char) c);
        try {
            current_node = decode_piece(current_node, s, special_case);  
        } catch (FileIOException& e) {
            throw e;
        }
    }

    s.clear();
    while (in->read(&c, 1)) {
        if (in->bad()) {
            throw FileIOException("reading");
        }
        s += byte_bin((unsigned char) c);
    }

    s = delete_end_zeros(s);

    try {
        current_node = decode_piece(current_node, s, special_case);
    } catch (FileIOException& e) {
        throw e;
    }
    
}

void huff::HuffDecoder::print_info() {
    in->clear(); in->seekg(0, in->end);
    std::cout << (int)in->tellg() - key_length * 5 - 5 << std::endl;
    std::cout << (int)out->tellp() << std::endl;
    std::cout << key_length * 5 + 5 << std::endl;
}
