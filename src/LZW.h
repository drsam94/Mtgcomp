// (c) 2017 Sam Donow
#pragma once
#include "Trie.h"
#include <cstddef>
#include <vector>
#include <string_view>

namespace Compress {

    template<typename numeric_t, size_t AlphabetSize>
    std::vector<numeric_t> compress(const std::string &s,
            const std::array<char, AlphabetSize> &alphabet, size_t &numBits) {
        Trie<numeric_t, AlphabetSize> trie(alphabet);
        numeric_t maxCode = 0;
        std::string oneChar(" ");
        for (char c : alphabet) {
            oneChar[0] = c;
            if (!trie.insert(oneChar, maxCode++)) {
                fprintf(stderr, "Failed to insert %s\n", oneChar.c_str());
            }
        }
        size_t ind = 0;
        std::vector<numeric_t> output;
        numBits = 0;
        // bitsPerCode = ceil(log2(AlphabetSize))
        int bitsPerCode = sizeof(size_t) * 8 - __builtin_clzl(AlphabetSize);
        while (ind < s.size()) {
            std::string_view sv(s);
            sv.remove_prefix(ind);
            auto [pLen, value] = trie.longestPrefix(sv);
            output.push_back(value);
            sv.remove_suffix(sv.size() - pLen - 1);
            if (maxCode >= (1 << bitsPerCode)) {
                ++bitsPerCode;
            }
            numBits += bitsPerCode;
            trie.insert(sv, maxCode++);
            ind += pLen;
        }
        return output;
    }
}
