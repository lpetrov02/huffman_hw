#pragma once

#include "huff.hpp"
#include "encoder.hpp"
#include "decoder.hpp"


namespace huff {

    class HuffTester {
    public:
        HuffTester() : Enc(nullptr), Dec(nullptr) {}

        void add_encoder(HuffEncoder* E) {
            Enc = E;
        }
        void add_decoder(HuffDecoder* D) {
            Dec = D;
        }

        void save_enc_order() {
            enc_nodes = Enc->T.nodes;
        }
        void save_dec_order() {
            dec_nodes = Dec->T.nodes;
        }

        bool check_nodes_order() {
            auto it1 = enc_nodes.begin();
            auto it2 = dec_nodes.begin();
            while (it1 != enc_nodes.end()) {
                if (it2 == dec_nodes.end())
                    return false;
                if ((*it1)->s != (*it2)->s)
                    return false;
                ++it1; ++it2;
            }
            if (it2 != dec_nodes.end())
                return false;
            return true;
        }

        bool check_trees_are_same(HuffNode* a, HuffNode* b) {
            bool R, L;
            if (a->right && a->left && b->right && b->left) {
                R = check_trees_are_same(a->right, b->right);
                L = check_trees_are_same(a->left, b->left);
                return R && L;
            } else if (!a->right && !a->left && !b->right && !b->left) {
                return (a->s == b->s) && (a->prob == b->prob);
            } else {
                return false;
            }
        }

        bool check_full_trees() {
            return check_trees_are_same(*(Enc->T.nodes.begin()), *(Dec->T.nodes.begin()));
        }

    private:
        huff::HuffEncoder* Enc;
        huff::HuffDecoder* Dec;

        std::set<HuffNode*, HuffComparator> enc_nodes;
        std::set<HuffNode*, HuffComparator> dec_nodes;
    };

}