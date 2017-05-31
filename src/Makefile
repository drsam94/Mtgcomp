all: main.cc LZW.h json.hpp Trie.h MagicProcessing.h
	g++ -o ../bin/magcomp main.cc -O3 --std=c++1z -Wall -Werror -Wextra -Wpedantic

debug: main.cc LZW.h json.hpp Trie.h MagicProcessing.h
	g++ -o ../bin/magcomp.debug main.cc -O0 -g --std=c++1z -Wall -Werror -Wextra -Wpedantic
