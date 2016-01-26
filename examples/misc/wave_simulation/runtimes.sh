#!/bin/sh

for prog in slow_wave fast_wave ghost_wave ptr_wave
do
	echo
	echo "Running $prog"
	time $prog 1024 2000 dump.$prog > $prog.out
done
