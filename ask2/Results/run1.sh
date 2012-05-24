#!/bin/bash
#script for running part 1

OUTF="results1.txt"

for i in `ls ../team03`
do
    echo "Running test $i"
    echo "$i" >> $OUTF
    ../src/predict ../team03/$i >> $OUTF
    echo " ">> $OUTF
done
