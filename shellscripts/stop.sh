#!/bin/bash

declare -a array
array=(`more frog.pid`)
pid=${array[1]}
path=${array[0]}

if test $path = `pwd`;then
	kill -9 $pid
	rm -f ./frog.pid
fi


