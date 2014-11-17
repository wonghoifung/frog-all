#!/bin/bash

bin=frog_server

export LD_LIBRARY_PATH+="/usr/local/lib:../boost_1_53_0/lib:"

declare -a array
array=(`more frog.pid`)
pid=${array[1]}
path=${array[0]}

if test $path = `pwd`;then
	kill -9 $pid
fi

#datetag=`date '+%m_%d_%k_%M'`

# open to handle log file
#if [ ! -d log ];then
#	mkdir log
#fi
#mv *.log ./log

nohup ./${bin} & 

