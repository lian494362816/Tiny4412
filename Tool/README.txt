arm-linux-gcc-4.5.1-v6-vfp-20120301.tgz
	交叉编译器

dnw-linux.tar.gz  
    通用dnw 工具

dnw-linux_ubuntu_16_04.tar.gz
    ubuntu 16.04 使用的dnw 工具
    使用步骤
        1, 解压 tar -xvf dnw-linux_ubuntu_16_04.tar.gz

        2,
            1)cd dnw-linux_ubuntu_16_04/src/driver 然后执行make
            2)插入模块 insmod secbulk.ko
            3)查看是否插入成功  lsmod | grep secbulk

        3,
            1)cd dnw-linux_ubuntu_16_04/src/dnw 然后执行make
            2)sudo cp dnw /bin

