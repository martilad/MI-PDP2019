#!/usr/bin/env bash

g++ -Wall -pedantic -O3  -fopenmp solvePOL.cpp
mkdir res

./a.out -f inst/pol0.txt -ls > ./res/res0.txt
./a.out -f inst/pol1.txt -ls > ./res/res1.txt
./a.out -f inst/pol2.txt -ls > ./res/res2.txt
./a.out -f inst/pol3.txt -ls > ./res/res3.txt
./a.out -f inst/pol4.txt -ls > ./res/res4.txt
./a.out -f inst/pol5.txt -ls > ./res/res5.txt
./a.out -f inst/pol6.txt -ls > ./res/res6.txt
./a.out -f inst/pol7.txt -ls > ./res/res7.txt
./a.out -f inst/pol8.txt -ls > ./res/res8.txt
./a.out -f inst/pol9.txt -ls > ./res/res9.txt
./a.out -f inst/pol10.txt -ls > ./res/res10.txt
./a.out -f inst/pol11.txt -ls > ./res/res11.txt

./a.out -f inst/pol0.txt -tp > ./res/res0p.txt
./a.out -f inst/pol1.txt -tp > ./res/res1p.txt
./a.out -f inst/pol2.txt -tp > ./res/res2p.txt
./a.out -f inst/pol3.txt -tp > ./res/res3p.txt
./a.out -f inst/pol4.txt -tp > ./res/res4p.txt
./a.out -f inst/pol5.txt -tp > ./res/res5p.txt
./a.out -f inst/pol6.txt -tp > ./res/res6p.txt
./a.out -f inst/pol7.txt -tp > ./res/res7p.txt
./a.out -f inst/pol8.txt -tp > ./res/res8p.txt
./a.out -f inst/pol9.txt -tp > ./res/res9p.txt
./a.out -f inst/pol10.txt -tp > ./res/res10p.txt
./a.out -f inst/pol11.txt -tp > ./res/res11p.txt

./a.out -f inst/pol0.txt -r > ./res/res0r.txt
./a.out -f inst/pol1.txt -r > ./res/res1r.txt
./a.out -f inst/pol2.txt -r > ./res/res2r.txt
./a.out -f inst/pol3.txt -r > ./res/res3r.txt
./a.out -f inst/pol4.txt -r > ./res/res4r.txt
./a.out -f inst/pol5.txt -r > ./res/res5r.txt
./a.out -f inst/pol6.txt -r > ./res/res6r.txt
./a.out -f inst/pol7.txt -r > ./res/res7r.txt
./a.out -f inst/pol8.txt -r > ./res/res8r.txt
./a.out -f inst/pol9.txt -r > ./res/res9r.txt
./a.out -f inst/pol10.txt -r > ./res/res10r.txt
./a.out -f inst/pol11.txt -r > ./res/res11r.txt