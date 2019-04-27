busybox-1.22.1
    这一版的有问题，复制完_install/* 后， 需要把linuxrc删除后，在重新复制进来
    原因分析:linuxrc只是一个链接文件，删除再复制进来就是不是链接文件了,
    可是1.17.2版本linuxrc也是一个链接，为什么就可以成功运行，目前原因不清楚

busybox-1.17.2
    这一版本是ok的

 busybox-1.30.0.tar.bz2
    busybox 1.30 版本

 busybox_1.30_patch
    1.30 版本对应的patch 


