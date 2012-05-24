#!/bin/bash
#script for running part 1

for j in {1..7}
do
    echo "$j-BIT predictor"
    #sed "s/NBP_TABLE_BITS\ [0-9]*/NBP_TABLE_BITS\ $j/1" nbit_predictor.h.bak > nbit_predictor.h
    sed "s/nbit_predictor(1/nbit_predictor($j/1" predict.cc.bak > predict.cc
    make
    for i in `ls ../team03`
    do
        echo "Running test $i"
        echo "$i" >> ../Results/part1/result$j
        predict ../team03/$i >> ../Results/part1/result$j
        echo " ">> ../Results/part1/result$j
    done
    echo " "
done
