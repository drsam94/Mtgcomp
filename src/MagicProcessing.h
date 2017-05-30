// (c) 2017 Sam Donow
#pragma once
#include <array>
#include <string>
#include <ctype.h>
#include <algorithm>
#include <stdio.h>

namespace MTG {
    constexpr const size_t AlphaSize = 60;
    constexpr const std::array<char, AlphaSize> alphabet = {
        'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p',
        'q','r','s','t','u','v','w','x','y','z',
        '-','+',' ','\n','{','}',':','.',',','*',
        '(',')','0', '1', '2','3','4','5','6','7','8','9',
        '\'', '"', '/', '\t', ';', '?', '&', '|', '_', '$', '%'
        };

    std::string preprocess(const std::string &s) {
        std::string ret;
        ret.reserve(s.size());
        std::vector<char> unicodeStack;
        for (char c : s) {
            if (auto it = std::find(std::begin(alphabet), std::end(alphabet), c);
                     it != std::end(alphabet)) {
                ret.push_back(c);
            } else if (c >= 'A' && c <= 'Z') {
                ret.push_back(tolower(c));
            } else if (unicodeStack.size() == 2 &&
                    unicodeStack[0] == -30 &&
                    (unicodeStack[1] == -128 || unicodeStack[1] == -120)) {
                if (c == -108) {
                    // em-dash
                    ret.push_back('|');
                } else if (c == -94) {
                    // bullet
                    ret.push_back('_');
                } else if (c == -110 || c == -104 || c == -98 || c == -109) {
                    // idk
                    ret.push_back('%');
                } else {
                    fprintf(stderr, "Bad unicode found in corpus: %d\n%s", (int)c, s.c_str());
                }
                unicodeStack.clear();
            } else if (unicodeStack.size() == 2 &&
                    unicodeStack[0] == -61 &&
                    unicodeStack[1] == -122 && c == -122) {
                ret.push_back('$');
                unicodeStack.clear();
            } else if (unicodeStack.size() < 2) {
                unicodeStack.push_back(c);
            } else { //if (unicodeStack.size() == 2) {
                // stupid other unicode chars
                ret.push_back('%');
                unicodeStack.clear();
                //fprintf(stderr, "Bad char found in corpus: %d[%d %d]\n%s", (int)c,
                //        (int)unicodeStack[0], (int)unicodeStack[1], s.c_str());
            }
        }
        return ret;
    }
}
