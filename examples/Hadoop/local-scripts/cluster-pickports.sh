#!/bin/sh

# This script modifies ports in config files to allow multiple instances of hadoop to 
# run on the same cluster using common namenode but separate datanodes.
# author: Marissa Hollingsworth and Amit Jain

HADOOP_HOME=$HOME/hadoop-install/hadoop

if test ! -d "${HADOOP_HOME}"
then
    echo
    echo "Error: missing hadoop install folder: ${HADOOP_HOME}"
    echo "Install hadoop in install folder before running this script!"
    echo
    exit 1
fi

if [ $# -ne 1 ]
then
    echo "Usage: $0 <base port>"
	exit 1
fi

baseport=$1
if [ $baseport -gt  62000 -o $baseport -lt 10000 ]
then
	echo "$0: bad base port range: choose between 10000 and 60000"
	exit 1
fi
if [ $baseport -eq  50000 ]
then
	echo "$0: forbidden base port range: 50000 is standard port so avoid it!"
	exit 1
fi

cd ${HADOOP_HOME}

/bin/cp conf/core-site.xml conf/core-site.xml.backup
/bin/cp conf/hdfs-site.xml conf/hdfs-site.xml.backup
/bin/cp conf/mapred-site.xml conf/mapred-site.xml.backup


sed -i "s/YOURUSERNAME/`whoami`/" conf/hdfs-site.xml
sed -i "s/YOURUSERNAME/`whoami`/" conf/mapred-site.xml

port=$baseport
param="fs.default.name"
newvalue="<value>hdfs://node00:$port</value>"
sed -i "/$param/ {
n
c\
$newvalue
}" conf/core-site.xml

port=$[baseport+1]
param="mapred.job.tracker"
newvalue="<value>hdfs://node00:$port</value>"
sed -i "/$param/ {
n
c\
$newvalue
}" conf/mapred-site.xml

port=$[baseport+30]
param="mapred.job.tracker.http.address"
newvalue="<value>0.0.0.0:$port</value>"
sed -i "/$param/ {
n
c\
$newvalue
}" conf/mapred-site.xml

port=$[baseport+60]
param="mapred.task.tracker.http.address"
newvalue="<value>0.0.0.0:$port</value>"
sed -i "/$param/ {
n
c\
$newvalue
}" conf/mapred-site.xml

port=$[baseport+10]
param="dfs.datanode.address"
newvalue="<value>0.0.0.0:$port</value>"
sed -i "/$param/ {
n
c\
$newvalue
}" conf/hdfs-site.xml

port=$[baseport+70]
param="dfs.http.address"
newvalue="<value>0.0.0.0:$port</value>"
sed -i "/$param/ {
n
c\
$newvalue
}" conf/hdfs-site.xml

port=$[baseport+90]
param="dfs.secondary.http.address"
newvalue="<value>0.0.0.0:$port</value>"
sed -i "/$param/ {
n
c\
$newvalue
}" conf/hdfs-site.xml

port=$[baseport+75]
param="dfs.datanode.http.address"
newvalue="<value>0.0.0.0:$port</value>"
sed -i "/$param/ {
n
c\
$newvalue
}" conf/hdfs-site.xml


port=$[baseport+20]
param="dfs.datanode.ipc.address"
newvalue="<value>0.0.0.0:$port</value>"
sed -i "/$param/ {
n
c\
$newvalue
}" conf/hdfs-site.xml

                
echo
echo "Updated files core-site.xml, hdfs-site.xml, mapred-site.xml in hadoop conf folder"
echo

