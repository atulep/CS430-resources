#!/bin/sh


echo "time mpiexec -n 8 io3 10240 /pvfs2/amit/testfile"
time mpiexec -n 8 io3 10240 /pvfs2/amit/testfile
echo

echo "time mpiexec -n 8 io2 10240 /tmp/testfile"
time mpiexec -n 8 io2 10240 /tmp/testfile
echo

echo "time mpiexec -n 8 io1 10240 testfile"
time mpiexec -n 8 io1 10240 testfile
echo
echo "time mpiexec -n 8 io2 10240 testfile"
time mpiexec -n 8 io2 10240 testfile
echo
echo "time mpiexec -n 8 io3 10240 testfile"
time mpiexec -n 8 io3 10240 testfile
echo
