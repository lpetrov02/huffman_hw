CXXFLAGS = -Wall -Werror -Wextra -g -std=c++17
OBJECTS = obj/main.o
TEST_OBJECTS = obj/huffman_test.o

.PHONY: all clean

all: hw-02_huffman

obj:
	mkdir obj
	chmod a=rwx obj

$(OBJECTS): obj/%.o: src/%.cpp
	g++ $(CXXFLAGS) -I include -c $< -o $@

main.o : huff_impl.hpp huff.hpp encoder.hpp encoder_impl.hpp decoder.hpp decoder_impl.hpp exceptions.hpp

$(TEST_OBJECTS): obj/%.o: test/%.cpp
	g++ $(CXXFLAGS) -I include -c $< -o $@

huffman_test.o : huff_impl.hpp huff.hpp encoder.hpp encoder_impl.hpp decoder.hpp decoder_impl.hpp exceptions.hpp

hw-02_huffman : obj $(OBJECTS)
	g++ $(CXXFLAGS) -o $@ $(OBJECTS)

huffman_test: obj $(TEST_OBJECTS)
	g++ $(CXXFLAGS) -o $@ $(TEST_OBJECTS)

clean :
	$(RM) -f -r obj hw-02_huffman huffman_test
