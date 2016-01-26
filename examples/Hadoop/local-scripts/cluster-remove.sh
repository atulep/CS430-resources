#!/bin/sh
# author: Amit Jain

cd $HOME/hadoop-install/hadoop

echo
echo "Warning: stopping adhoc cluster and deleting the hadoop filesystem!!"
echo -n "!!!!!!!!!!! Are you sure (y/n): "
read response
echo
if test "$response" = "y"
then
	bin/stop-all.sh
	/bin/rm -fr logs pids
	echo
	echo "Removing hadoop filesystem directories"
	pdsh -w - < conf/slaves  /bin/rm -fr /tmp/hadoop-`whoami`*
	rm -fr /tmp/hadoop-`whoami`*
	echo
fi

