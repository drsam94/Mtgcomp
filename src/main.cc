// (c) Sam Donow 2017
#include "json.hpp"
#include "MagicProcessing.h"
#include "util.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using json = nlohmann::json;

void dieUsage(char **argv)
{
    fprintf(stderr, "usage: %s [-O] [-p prefix] [-c color] jsonFile\n", argv[0]);
    exit(1);
}

int main (int argc, char **argv)
{
    int opt;
    bool useOracle = false;
    string outdir;
    char color = '\0';
    while ((opt = getopt(argc, argv, "Op:c:")) != -1) {
        switch (opt) {
            case 'O':
                useOracle = true;
                break;
            case 'p':
                outdir = optarg;
                break;
            case 'c':
                color = optarg[0];
                break;
            default:
                dieUsage(argv);
        }
    }
    if (optind >= argc) {
        dieUsage(argv);
    }
    ifstream file(argv[optind]);
    string s(static_cast<stringstream const&>(stringstream() << file.rdbuf()).str());
    json allSets = json::parse(s);

    // Compress the data, collated by set
    auto data = MTG::processData(allSets, useOracle, color);
    MTG::outputGraph(data, outdir);
    return 0;
}
