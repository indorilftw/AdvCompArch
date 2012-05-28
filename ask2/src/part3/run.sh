#!/bin/bash
#script for running part 1

for i in `ls ../../team03`
do
    echo "Running test $i"
    echo "$i" >> ../../Results/part3/result
    predict ../../team03/$i >> ../../Results/part3/result
    echo " ">> ../../Results/part3/result
done
echo ""
cat ../../Results/part3/result
