#!/bin/bash
#script for running part 1

for i in `ls ../../team03`
do
    echo "Running test $i"
    echo "$i" >> ../../Results/part2/result
    predict ../../team03/$i >> ../../Results/part2/result
    echo " ">> ../../Results/part2/result
done
echo " "
