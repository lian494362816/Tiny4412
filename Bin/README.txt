
=============Tiny4412 3.5内核, 友善之臂官方
E4412_N.bl1.bin
    制作SD卡时的BL1数据
bl2.bin
    制作SD卡时的BL2数据

E4412_tzsw.bin
    制作SD卡时的TrustZone数据

mkbl2
    把uboot.bin 变成bl2.bin 的程序

u-boot.bin
    事先编译好的uboot

sd_fusing.sh
    制作SD卡的脚本

zImage
    事先编译好的zImage


==========Tiny4412 4.4内核 带有dts
linux_4.4

    E4412_N.bl1.bin
        制作SD卡时的BL1数据
    bl2.bin
        制作SD卡时的BL2数据

    E4412_tzsw.bin
        制作SD卡时的TrustZone数据

    mkbl2
        把uboot.bin 变成bl2.bin 的程序

    u-boot.bin
        事先编译好的uboot, 针对设备树做了改动

    sd_fusing.sh
        制作SD卡的脚本

    uImage
        事先编译好的uImage, 带有设备树

    ramdisk.img
        文件系统

    exynos4412-tiny4412.dtb
        设备配置

    SD卡制作
        1. sudo dd if=/dev/zero of=/dev/mmcblk0 bs=1024 count=10240
        2. sudo ./sd_fusing.sh  /dev/mmcblk0
    
    下载步骤,使用dnw 工具
        uImage                  0x41000000
        ramdisk.img             0x42000000
        exynos4412-tiny4412.dtb 0x43000000 

    下载完后执行下面的命令启动内核 
        bootm 0x41000000 0x42000000 0x43000000

