#!/bin/bash

for linesz in 32 128
do
    for csize in 16 32 64
    do
        for cassoc in 4 8
        do
            ../../simics -stall -e '$LINESZ'=$linesz -e '$CSIZE'=$csize -e '$CASSOC'=$cassoc -x mcf.simics &> ../../RESULTS/part1.2/mcf-$linesz.$csize.$cassoc
        done
    done
done
