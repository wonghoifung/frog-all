#!/bin/bash

# TODO pack lib also

if [ $# != 1 ]; then
echo "usage: ./pack.sh [version]"
exit 1
fi

version=$1
bin="./frog_server"
bin_md5=${bin}'.md5'
System="CentOS"

datetag=`date '+%m%d%H%M'`

rm -fr $bin.$System.*.tar.gz

echo ${datetag}

packet="${bin}.${System}.${datetag}.${version}.tar.gz"

md5sum -b ${bin} > ${bin_md5}

tar -czf ${packet} ${bin} ${bin_md5}

rm -fr ${bin_md5} 


