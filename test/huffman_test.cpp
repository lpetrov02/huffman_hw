#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <utility>
#include <sstream>
#include <string>
#include <vector>

#include "huffman_test.hpp"


TEST_CASE("testing the HuffNode class") {
    huff::HuffNode n1('l', 0.2);
    huff::HuffNode n2('p', 0.25);
    huff::HuffNode n3('s', 0.35);
    huff::HuffNode n4('g', 0.1);
    huff::HuffNode n5(' ', 0.1);  

    CHECK(n4 < n1);
    CHECK(!(n3 < n2));
    CHECK(!(n5 < n4));
}

TEST_CASE("testing Encoder and Decoder - non-empty case") {
    std::vector<std::string> strings;

    std::string str1("mama myla ramu rama myla mamu!");
    std::string str2("Hello my name is Leonya.\nI study is spbu)\n");
    std::string str3("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    std::string str4("А слабо на русском?");
    std::string str5("Hey encoder! Ты не билингв случаем?");

    std::ostringstream ss;
    for (char i = 0; i < 100; i++)
        ss.write(&i, 1);
    std::string str6 = ss.str();

    strings.push_back(str1);
    strings.push_back(str2);
    strings.push_back(str3);
    strings.push_back(str4);
    strings.push_back(str5);
    strings.push_back(str6);

    for (auto it = strings.begin(); it != strings.end(); ++it) {
        std::string str = *it;
        std::istringstream in(str);
        std::ostringstream out;

        huff::HuffTester Tester;

        huff::HuffEncoder Encoder((std::istream*)&in, (std::ostream*)&out);
        Tester.add_encoder(&Encoder);

        Encoder.count_chars();
        Encoder.restart_input();

        Encoder.dict_to_tree();
        Tester.save_enc_order(); // запомнили, в каком порядке ноды записались в дерево

        Encoder.buid_tree();
        REQUIRE(Encoder.get_tree_size() == 1);
        Encoder.build_codes();
        Encoder.encode();

        std::string result = out.str();
        std::istringstream in2(result);
        std::ostringstream out2;

        huff::HuffDecoder Decoder((std::istream*)&in2, (std::ostream*)&out2);
        Tester.add_decoder(&Decoder);

        Decoder.get_key_make_tree();
        Tester.save_dec_order(); // запомнили, в каком порядке ноды записались в дерево

        bool nodes_in_the_same_order = Tester.check_nodes_order();
        CHECK(nodes_in_the_same_order);

        Decoder.build_tree();

        bool check_trees = Tester.check_full_trees();
        CHECK(check_trees);

        Decoder.decode();

        CHECK(str == out2.str());
    }
}


TEST_CASE("testing Encoder and Decoder - empty case") {
    std::string str("");

    std::istringstream in(str);
    std::ostringstream out;

    huff::HuffEncoder Encoder((std::istream*)&in, (std::ostream*)&out);
    Encoder.count_chars();
    Encoder.restart_input();
    Encoder.dict_to_tree();

    Encoder.buid_tree();
    REQUIRE(Encoder.get_tree_size() == 0);

    Encoder.build_codes();
    Encoder.encode();

    std::string result = out.str();
    std::istringstream in2(result);
    std::ostringstream out2;

    huff::HuffDecoder Decoder((std::istream*)&in2, (std::ostream*)&out2);

    Decoder.get_key_make_tree();
    Decoder.build_tree();
    Decoder.decode();

    CHECK(str == out2.str());
}

