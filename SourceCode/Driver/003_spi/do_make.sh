#!/bin/bash

dir_path=/home/black/Tiny4412/filesystem

if [[ $1 = "clean" ]] ;then
    make clean
    make -C drv_test clean
    rm $dir_path/drv_test -f
    rm $dir_path/*.ko -f
    
else
    make
    cp *.ko $dir_path
    cp drv_test/lib/libiconv.a_bk  drv_test/lib/libiconv.a
    make -C  drv_test
    cp ./drv_test/drv_test $dir_path
fi
