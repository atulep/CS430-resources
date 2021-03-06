#!/bin/bash
# author: Amit Jain

HADOOP_HOME=$HOME/hadoop-install/hadoop

if test ! -d "${HADOOP_HOME}"
then
	echo
	echo "Error: missing hadoop install folder: ${HADOOP_HOME}"
	echo "Install hadoop in install folder before running this script!"
	echo
	exit 1
fi

case $# in
0) echo "Usage: $0 <standalone|pseudo-distributed|distributed>"; exit 1;;
esac

cd templates/

if test "${1:0:1}" == "s"
then
	echo "Setting up config files for standalone mode."
	cp core-site.xml.standalone ${HADOOP_HOME}/conf/core-site.xml
	cp hdfs-site.xml.standalone ${HADOOP_HOME}/conf/hdfs-site.xml
	cp mapred-site.xml.standalone ${HADOOP_HOME}/conf/mapred-site.xml
	cp masters.standalone ${HADOOP_HOME}/conf/masters
	cp slaves.standalone ${HADOOP_HOME}/conf/slaves
	exit
fi

if test "${1:0:1}" == "p"
then
	echo "Setting up config files for pseudo-distributed mode."
	cp core-site.xml.pseudo-distributed ${HADOOP_HOME}/conf/core-site.xml
	cp hdfs-site.xml.pseudo-distributed ${HADOOP_HOME}/conf/hdfs-site.xml
	cp mapred-site.xml.pseudo-distributed ${HADOOP_HOME}/conf/mapred-site.xml
	cp masters.pseudo-distributed ${HADOOP_HOME}/conf/masters
	cp slaves.pseudo-distributed ${HADOOP_HOME}/conf/slaves
	exit
fi

if test "${1:0:1}" == "d"
then
	echo "Setting up config files for distributed mode."
	cp core-site.xml.distributed ${HADOOP_HOME}/conf/core-site.xml
	cp hdfs-site.xml.distributed ${HADOOP_HOME}/conf/hdfs-site.xml
	cp mapred-site.xml.distributed ${HADOOP_HOME}/conf/mapred-site.xml
	cp masters.pseudo-distributed ${HADOOP_HOME}/conf/masters
	cp slaves.pseudo-distributed ${HADOOP_HOME}/conf/slaves
	exit
fi

echo $0": Unknown option...try again"
