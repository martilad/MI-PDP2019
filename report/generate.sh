#!/bin/bash

startFileN=1
endFileN=1

mkdir o 2> /dev/null
mkdir e 2> /dev/null

switch="-mpi"

nTb=1
nTm=30
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
    epath="./e/pol${startFileN}"
    opath="./o/pol${startFileN}"

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
					sed -i "s|#{6}|${epath}_${switch:1}_T${nT}_NT${nN}_NP${nNP}_rep${rep}|" parallel_job.sh
					sed -i "s|#{7}|${opath}_${switch:1}_T${nT}_NT${nN}_NP${nNP}_rep${rep}|" parallel_job.sh
					echo "$nT - $nN - $nNP - $switch - $rep - $file - ${epath}_${switch:1}_T${nT}_NT${nN}_NP${nNP}_rep${rep} - ${opath}_${switch:1}_T${nT}_NT${nN}_NP${nNP}_rep${rep}"
					echo "RUN TO QRUN"
					rm parallel_job.sh
    			done
    		done
    	done
    done
done