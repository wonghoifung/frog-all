bin=chat

export LD_LIBRARY_PATH+="/usr/local/lib:../../../boost_1_53_0/lib/:../../:../../../mysql-connector-c-6.0.2-linux-glibc2.3-x86-64bit/lib/:../../../postgresql-9.3.5/lib/:"

declare -a array
array=(`more frog.pid`)
pid=${array[1]}
path=${array[0]}

#if test $path = `pwd`;then
#	kill -9 $pid
#fi

#if [ ! -d log ];then
#	mkdir log
#fi
#mv *.log ./log

#nohup ./${bin}  & 

./${bin}
