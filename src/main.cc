// (c) Sam Donow 2017
#include "json.hpp"
#include "LZW.h"
#include "MagicProcessing.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
using namespace std;


using json = nlohmann::json;
int main (int argc, char **argv)
{
    if (argc < 2) {
        printf("usage: prog data.json");
    }
    // Parse the set data
    ifstream file(argv[1]);
    string s(static_cast<stringstream const&>(stringstream() << file.rdbuf()).str());
    auto allSets = json::parse(s);

    // Compress the data, collated by set
    for (auto it = allSets.begin(); it != allSets.end(); ++it) {
        auto set = it.value();
        string corpus;
        for (auto card : set["cards"]) {
            // Vanilla creatures don't have text!
            if (auto it = card.find("originalText"); it != card.end()) {
                corpus += MTG::preprocess(it.value().get<string>());
            }
        }
        if (corpus.empty()) {
            continue;
        }
        size_t bitsCompressed;
        auto compressed = Compress::compress<uint16_t>(corpus, MTG::alphabet, bitsCompressed);
        const size_t bitsRaw = corpus.size() * 8;
        const double ratio = double(bitsCompressed) / bitsRaw;
        printf("%s: %lu -> %lu (%g%%)\n", set["code"].get<string>().c_str(),
                bitsRaw / 8, bitsCompressed / 8, ratio);
    }
    return 0;
}
