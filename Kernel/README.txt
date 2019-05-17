
linux-3.5-20151029.tgz
    友善之臂提供的kernel

    编译步骤
     1 cp tiny4412_linux_defconfig   .config
     2 vim  .config, 确保  485 # CONFIG_ARM_TRUSTZONE is not set  已经注释掉
     3 make

linux-4.4.tar.xz
    支持Tiny4412 设备树的kernel (高版本的都支持了)
    编译方法
        1) 打patch 
            cd linux-4.4
            patch -p1 < ../linux-4.4_device_tree.patch

        2) 编译uImage
            (使用patch 就不要不执行, 否则会覆盖.config)make exynos_defconfig
            make uImage LOADADDR=0x40008000 -j4
            arch/arm/boot 下生成uImage

        3) 编译dts
            make dtbs
            arch/arm/boot/dts/ 下生成exynos4412-tiny4412.dtb

linux-4.4_device_tree.patch
    linux-4.4 带有设备树配置的patch 
