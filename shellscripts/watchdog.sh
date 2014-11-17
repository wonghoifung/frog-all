#!/bin/bash

while true 
do

pid=`cat ./frog.pid | awk '{print $2}'`
#path=`more ./frog.pid | awk '{print $1}'`

bin_pid=`ps -ef|awk '{print $2}' | grep -w ${pid}`
if ! test -n "${bin_pid}"; then
	./restart.sh
fi

sleep 30
done

