#!/usr/bin/env bash

g++ solvePOL.cpp
mkdir res
./a.out -i < ./inst/pol0.txt > ./res/res0.txt
./a.out -i < ./inst/pol1.txt > ./res/res1.txt
./a.out -i < ./inst/pol2.txt > ./res/res2.txt
./a.out -i < ./inst/pol3.txt > ./res/res3.txt
./a.out -i < ./inst/pol4.txt > ./res/res4.txt
./a.out -i < ./inst/pol5.txt > ./res/res5.txt
./a.out -i < ./inst/pol6.txt > ./res/res6.txt
./a.out -i < ./inst/pol7.txt > ./res/res7.txt
./a.out -i < ./inst/pol8.txt > ./res/res8.txt
./a.out -i < ./inst/pol9.txt > ./res/res9.txt
./a.out -i < ./inst/pol10.txt > ./res/res10.txt
./a.out -i < ./inst/pol11.txt > ./res/res11.txt
