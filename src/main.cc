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
        printf("usage: %s data.json", argv[0]);
    }
    // Parse the set data
    ifstream file(argv[1]);
    string s(static_cast<stringstream const&>(stringstream() << file.rdbuf()).str());
    json allSets = json::parse(s);

    vector<tuple<size_t, size_t, const json *>> data;
    // Compress the data, collated by set
    for (auto it = allSets.begin(); it != allSets.end(); ++it) {
        json &set = it.value();
        string corpus;
        for (const json &card : set["cards"]) {
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
        data.push_back({bitsRaw, bitsCompressed, &set});
    }
    // sort by set release date
    sort(data.begin(), data.end(), [](const auto &a, const auto &b)
            {
                auto getDate = [](decltype(a) tup) {
                    const auto &[dummy1, dummy2, set] = tup;
                    (void)dummy1, (void)dummy2;
                    char buf[9];
                    char *p = buf;
                    for (char c : (*set)["releaseDate"].dump()) {
                        if (c != '-' && c != '"') {
                            *(p++) = c;
                        }
                    }
                    *p = '\0';
                    return atoi(buf);
                };
                return getDate(a) < getDate(b);
            });
    for (auto &[bitsRaw, bitsCompressed, set] : data) {
        const double ratio = double(bitsCompressed) / bitsRaw;

        printf("%s: %lu -> %lu (%g%%)\n", (*set)["code"].get<string>().c_str(),
                bitsRaw / 8, bitsCompressed / 8, ratio);
    }
    return 0;
}
