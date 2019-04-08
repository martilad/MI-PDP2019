#!/bin/bash

startFileN=1
endFileN=10

mkdir o 2> /dev/null
mkdir e 2> /dev/null

switch="-mpi"
nT=20
nN=20
nNP=20

for ((startFileN ; startFileN <= $endFileN ; startFileN++)); do
    file=inst/pol${startFileN}.txt
    epath="./o/$startFileN"
    opath="./o/$startFileN"

    startP=1
    endP=10
    stepP=2
    for ((startP ; startP <= $endP ; startP=startP+stepP)); do
        echo "$startP"
    done

    echo "Counter: $file"
done

# nahrazovani v souboru
cp parallel_job_temp.sh parallel_job.sh
sed -i 's/#{1}/1/' parallel_job.sh