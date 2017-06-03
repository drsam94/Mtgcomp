// (c) 2017 Sam Donow
#pragma once
#include "LZW.h"
#include "json.hpp"
#include "util.h"
#include <tuple>
#include <array>
#include <string>
#include <ctype.h>

namespace MTG {
    constexpr const size_t AlphaSize = 60;
    constexpr const std::array<char, AlphaSize> alphabet = {
        'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p',
        'q','r','s','t','u','v','w','x','y','z',
        '-','+',' ','\n','{','}',':','.',',','*',
        '(',')','0', '1', '2','3','4','5','6','7','8','9',
        '\'', '"', '/', '\t', ';', '?', '&', '|', '_', '$', '%'
        };

    std::string preprocess(const std::string &s);

    // Process a JSON file and get the data on how well it compressed
    std::vector<std::tuple<size_t, size_t, const nlohmann::json *>>
    processData(const nlohmann::json &allSets, bool oracle);
}

