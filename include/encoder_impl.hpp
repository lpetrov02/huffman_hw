#pragma once

#include "encoder.hpp"
#include "exceptions.hpp"


unsigned char huff::code_to_int(std::string* s, int pos) {
    unsigned char res = 0;
    int mult = 1;

    for (int i = pos + 7; i >= pos; i--) {
        res += (int)((char)(*s)[i] == '1') * mult;
        mult *= 2;
    }

    return res;
}


huff::HuffEncoder::HuffEncoder(std::istream* input, std::ostream* output) {
    in = input;
    out = output;
    
    buff_size = TRESHOLD;

    dict = std::map<char, int>();

    codes = new std::map<char, std::string>;
    if (!codes) {
        throw MemoryException("std::map<char, std::string>");
    }

    T = huff::HuffTree();
    total = 0;
}

huff::HuffEncoder::~HuffEncoder() {
    delete codes;
}

void huff::HuffEncoder::count_chars() {
    char c;

    while (in->read(&c, 1)) {
        if (in->bad()) {
            throw FileIOException("reading");
        }
        auto it = dict.find(c);
        if (it == dict.end()) {
            dict.insert({c, 1});
        }
        else {
            it->second++;
        }
    }

    in->clear(); in->seekg(0, in->end);
    total = in->tellg();
}

void huff::HuffEncoder::dict_to_tree() {
    for (auto it = dict.begin(); it != dict.end(); it++) {
        try {
            T.add(new huff::HuffNode(it->first, (float)it->second / (float)total));
        } catch (MemoryException& e) {
            throw e;
        }
    }
}

void huff::HuffEncoder::buid_tree() {
    while (T.size() > 1) {
        std::pair<huff::HuffNode*, huff::HuffNode*> p = T.find_two_min();

        try {
            T.unite(p.first, p.second);
        } catch (MemoryException& e) {
            throw e;
        }
    }
}

void huff::HuffEncoder::build_codes() {
    T.build_codes(codes);
}

void huff::HuffEncoder::encode_key() {
    int key_size = dict.size();
    out->write((char*)&key_size, 4);
    if (out->bad()) {
        throw FileIOException("writing");
    }

    auto it = dict.begin();
    while (it != dict.end()) {
        float prob = (float)it->second / (float)total;
        out->write(&(it->first), 1);
        if (out->bad()) {
            throw FileIOException("writing");
        }
        out->write((char*)&prob, 4);
        if (out->bad()) {
            throw FileIOException("writing");
        }
        ++it;
    }
}

void huff::HuffEncoder::write_iteration(std::string accum, size_t max_pos) {
    for (size_t pos = 0; pos < max_pos; pos += 8) {
        char code_num = huff::code_to_int(&accum, pos);
        out->write(&code_num, 1);
        if (out->bad()) {
            throw FileIOException("writing");
        }
    }
}

void huff::HuffEncoder::print_info() {
    in->clear(); in->seekg(0, in->end);
    std::cout << (int)in->tellg() << std::endl;
    std::cout << (int)out->tellp() - dict.size() * 5 - 5 << std::endl;
    std::cout << dict.size() * 5 + 5 << std::endl;
}

int huff::HuffEncoder::encode() {
    std::string accum, s;
    char c;
    try {
        encode_key();
    } catch (FileIOException& e) {
        throw e;
    }

    while (in->read(&c, 1)) {
        accum += (*codes)[c];
        if (accum.length() > buff_size) {
            for (size_t i = buff_size; i < accum.length(); i++)
                s.push_back(accum[i]);
            try {
                write_iteration(accum, buff_size);
            } catch (FileIOException& e) {
                throw e;
            }
            accum = s;
            s.clear();
        }
    }

    char end_zeros = (char)((8 - accum.length() % 8) % 8);
    if (accum.length() % 8 != 0) {
        accum += std::string((int)end_zeros, '0');
    }
    try {
        write_iteration(accum, accum.length());
    } catch (FileIOException& e) {
        throw e;
    }

    out->write(&end_zeros, 1);
    if (out->bad()) {
        throw FileIOException("writing");
    }


    return 0;
}

void huff::HuffEncoder::restart_input() {
    in->clear();
    in->seekg(0, in->beg);
}

int huff::HuffEncoder::get_tree_size() {
    return T.size();
}