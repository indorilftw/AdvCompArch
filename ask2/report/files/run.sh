#!/bin/bash
#script for running part 1

for i in `ls ../../team03`
do
    echo "Running test $i"
    echo "$i" >> ../../Results/part1/result
    predict ../../team03/$i >> ../../Results/part1/result
    echo " ">> ../../Results/part1/result
done
echo ""
cat ../../Results/part1/result
