#include "MagicProcessing.h"
#include <stdio.h>
#include <algorithm>
using namespace std;
using json = nlohmann::json;

string MTG::preprocess(const string &s) {
    string ret;
    ret.reserve(s.size());
    vector<char> unicodeStack;
    for (char c : s) {
        if (auto it = find(begin(alphabet), end(alphabet), c);
                 it != end(alphabet)) {
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
                fprintf(stderr, "Bad unicode found in corpus: %d\n%s", (int)c, +s);
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
        }
    }
    return ret;
}

// Process a JSOON file and get the data on how well it compressed
vector<tuple<size_t, size_t, const json *>>
MTG::processData(const json &allSets, bool oracle) {
    vector<tuple<size_t, size_t, const json *>> data;
    for (auto it = allSets.begin(); it != allSets.end(); ++it) {
        const json &set = it.value();
        if (set["type"] != "core" && set["type"] != "expansion") continue;
        string corpus;
        for (const json &card : set["cards"]) {
            // Vanilla creatures don't have text!
            if (auto it = card.find(oracle ? "text" : "originalText"); it != card.end()) {
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
    return data;
}


