#!/bin/sh
if [ ! -f data/AllSets-x.json ]
then
    cd data;
    ./getData.sh;
    unzip AllSets-x.json.zip;
    cd ..
fi

if [ ! -e out ]
then
    mkdir out
fi

if [ ! -e bin ]
then
    mkdir bin
fi

cd src
make all
cd ..
bin/magcomp -p out/raw_ data/AllSets-x.json
if [ $? -ne 0 ]
then
    echo "script failed"
    exit 1
fi
bin/magcomp -O -p out/oracle_ data/AllSets-x.json
if [ $? -ne 0 ]
then
    echo "script failed"
    exit 1
fi
gnuplot out/raw_commands.gnuplot
gnuplot out/oracle_commands.gnuplot
