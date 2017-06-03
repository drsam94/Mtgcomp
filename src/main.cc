// (c) Sam Donow 2017
#include "json.hpp"
#include "MagicProcessing.h"
#include "util.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
using namespace std;
using json = nlohmann::json;

void dieUsage(char **argv)
{
    fprintf(stderr, "usage: %s [-O] [-p prefix] jsonFile", argv[0]);
    exit(1);
}

int main (int argc, char **argv)
{
    int opt;
    bool useOracle = false;
    string outdir;
    while ((opt = getopt(argc, argv, "Op:")) != -1) {
        switch(opt) {
            case 'O':
                useOracle = true;
                break;
            case 'p':
                outdir = optarg;
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
    vector<tuple<size_t, size_t, const json *>> data = MTG::processData(allSets, useOracle);

    FileOpen dataFile(+(outdir + "data.dat"), "w");
    unsigned idx = 0;
    for (auto &[bitsRaw, bitsCompressed, set] : data) {
        const double ratio = double(bitsCompressed) / bitsRaw;
        const char *setName = +(*set)["code"].get<string>();
        fprintf(dataFile.get(), "%u %s %g\n", idx++, setName, ratio);
    }

    FileOpen gnuplotFile(+(outdir + "commands.gnuplot"), "w");
    fprintf(gnuplotFile.get(), "set terminal pngcairo size 4096, 1024\n"
            "set output '%sgraph.png'\n"
            "set boxwidth 0.5\n"
            "set style fill solid\n"
            "f(x) = a*x + b\n"
            "fit f(x) '%sdata.dat' using 1:3 via a,b\n"
            "stats '%sdata.dat' using (f($0)):2 prefix 'A'\n"
            "set label 1 at graph 0.1, graph 0.85 sprintf('r = %%4.2f', A_correlation) center offset 0, 1\n"
            "plot '%sdata.dat' using 1:3:xtic(2) with boxes title 'Compression Ratio'"
            ", f(x) title 'Linear Fit' lw 6\n",
            +outdir, +outdir, +outdir, +outdir);
    return 0;
}
