#!/bin/sh

case $# in
0) echo "Usage: test.sh <program> <#elements>"; exit 1;;
esac

prog=$1
count=$2

for i in 1 2 3 4 
do
	export OMP_NUM_THREADS=$i
	echo
	echo "Running with $i threads"
	time ./$prog $count 
done
