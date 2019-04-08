#!/bin/bash

startFileN=1
endFileN=10

mkdir o 2> /dev/null
mkdir e 2> /dev/null

switch="-mpi"

nTb=1
nTm=1
nTs=1

nNb=20
nNm=20
nNs=20

nNPb=20
nNPm=20
nNPs=20

repeat=0

for ((startFileN ; startFileN <= $endFileN ; startFileN++)); do
    file=inst/pol${startFileN}.txt
    epath="./o/$startFileN"
    opath="./o/$startFileN"

    nT=$nTb
    for ((nT ; nT <= $nTm ; nT=nT+nTs)); do
    	nN=$nNb
		for ((nN ; nN <= $nNm ; nN=nN+nNs)); do
			nNP=$nNPb
			for ((nNP ; nNP <= $nNPm ; nNP=nNP+nNPs)); do
				rep=0
				for ((rep ; rep <= $repeat ; rep++)); do
					# nahrazovani v souboru
					cp parallel_job_temp.sh parallel_job.sh
					sed -i "s|#{1}|$switch|g" parallel_job.sh
					sed -i "s|#{2}|$file|g" parallel_job.sh
					sed -i "s|#{3}|$nT|" parallel_job.sh
					sed -i "s|#{4}|$nN|" parallel_job.sh
					sed -i "s|#{5}|$nNP|" parallel_job.sh
					sed -i "s|#{6}|$switch|" parallel_job.sh
					sed -i "s|#{7}|$switch|" parallel_job.sh
					echo "$nT - $nN - $nNP - $switch - $rep"
    			done
    		done
    	done
    done

    echo "Counter: $file"
done

# nahrazovani v souboru
#cp parallel_job_temp.sh parallel_job.sh
#sed -i 's/#{1}/1/' parallel_job.sh