
filesystem.tar.gz
    制作好的文件系统

libiconv-1.16.tar.gz
    iconv 库， 可以将utf-8 转为 gbk

    编译方法
        1) 创建安装目录 mkdir libiconv_build
        2) 执行配置 ./configure --prefix=/home/black/Tiny4412/TAR/libiconv_build CC=arm-linux-gcc --host=arm 
        3) 编译 make && make install
