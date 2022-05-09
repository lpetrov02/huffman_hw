#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>

#include "huff.hpp"
#include "exceptions.hpp"

#define UNUSED(x) (void)(x)
#define TRESHOLD 400


// Huffman Tree Node class methods #########################################

huff::HuffNode::HuffNode(std::string str, float p, huff::HuffNode* r, huff::HuffNode* l) {
    s = str;
    prob = p;
    right = r;
    left = l;
}

huff::HuffNode::HuffNode(char c, float p) {
    s = std::string(1, c);
    prob = p;
    right = left = nullptr;
}

huff::HuffNode::~HuffNode() {
    if (right != nullptr)
        delete right;
    if (left != nullptr)
        delete left;
}

bool huff::HuffNode::operator<(const huff::HuffNode& other) const {
    return prob < other.prob;
}

void huff::HuffNode::print(int depth) const {
    if (right != nullptr)
        right->print(depth + 1);
    std::string shift = std::string(depth * 15, ' ');
    std::cout << shift << "(" << s << ": " << prob << ")" << std::endl;
    if (left != nullptr)
        left->print(depth + 1);
}

void huff::HuffNode::build_codes(std::map<char, std::string>* codes, std::string str) const {
    if (right == nullptr && left == nullptr) { // Вторая проверка для симметрии
        if (str.length() == 0) {
            codes->insert({s[0], std::string("1")});
        }
        else {
            codes->insert({s[0], str});
        }
    }

    if (right != nullptr)
        right->build_codes(codes, str + std::string("1"));

    if (left != nullptr)
        left->build_codes(codes, str + std::string("0"));
}


// Huffman Tree class methods #################################

huff::HuffTree::HuffTree() {
    nodes = std::set<huff::HuffNode*, huff::HuffComparator>();
}

huff::HuffTree::~HuffTree() {   
    auto it = nodes.begin();
    while (it != nodes.end()) {
        delete *it++;
    }
}

void huff::HuffTree::add(huff::HuffNode* node) {
    if (!node) {
        throw MemoryException("HuffNode*");
    }
    nodes.insert(nodes.begin(), node);
}

void huff::HuffTree::unite(huff::HuffNode* first, huff::HuffNode* second) {
    auto it1 = nodes.find(first);
    auto it2 = nodes.find(second);
    if (it1 == nodes.end() || it2 == nodes.end())
        return;
    
    try {
        add(new huff::HuffNode((*it1)->s + (*it2)->s, (*it1)->prob + (*it2)->prob, *it1, *it2));
    } catch (MemoryException& e) {
        throw e;
    }
    
    nodes.erase(*it1);
    nodes.erase(*it2);
}

std::pair<huff::HuffNode*, huff::HuffNode*> huff::HuffTree::find_two_min() const {
    std::pair<huff::HuffNode*, huff::HuffNode*> p;
    auto it = nodes.begin();

    p.first = *it++;
    p.second = *it++;
    if (*(p.second) < *(p.first)) {
        auto tmp = p.first;
        p.first = p.second;
        p.second = tmp;
    }

    for (; it != nodes.end(); it++) {
        if (**it < *(p.first)) {
            p.second = p.first;
            p.first = *it;
        }
        else if (**it < *(p.second)) {
            p.second = *it;
        }
    }
    return p;
}

void huff::HuffTree::print() const {
    auto it = --(nodes.end());
    do {
        (*it)->print(0);
    } while (it-- != nodes.begin());
}

int huff::HuffTree::size() const {
    return nodes.size();
}

void huff::HuffTree::build_codes(std::map<char, std::string>* codes) const{
    if (nodes.size() != 1)
        return;

    auto it = nodes.begin();
    huff::HuffNode* node = *it;
    node->build_codes(codes, std::string(""));
}
