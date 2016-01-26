#!/bin/sh

case $# in
0) echo "Usage: test.sh <#elements>"; exit 1;;
esac

count=$1

for i in 1 2 3 4 5 6 7 8 12 16 20 24
do
	export OMP_NUM_THREADS=$i
	echo "Running with $i threads"
	time ./openmp_sum $count
done
