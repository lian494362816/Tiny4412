uboot_tiny4412-master.zip
    友善之臂提供的uboot-2010.12,包含了制作sd卡的工具

    uboot写入SD卡的步骤
        1 cd uboot_tiny4412-master
        2 make tiny4412_config
        3 make
        4 cd uboot_tiny4412-master/sd_fuse
        5 make
        6 cd uboot_tiny4412-master/sd_fuse/tiny4412
        7 sudo ./sd_fusing.sh  /dev/mmcblk0 

uboot_kernel_4.4_device_tree.patch
    支持kernel-4.4 设备树功能的patch
