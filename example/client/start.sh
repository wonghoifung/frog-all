bin=client

export LD_LIBRARY_PATH+="/usr/local/lib:../../../boost_1_53_0/lib/:../../:../../../mysql-connector-c-6.0.2-linux-glibc2.3-x86-64bit/lib/:../../../postgresql-9.3.5/lib/:"

./${bin}   
