
linux-3.5-20151029.tgz
    友善之臂提供的kernel

    编译步骤
     1 cp tiny4412_linux_defconfig   .config
     2 vim  .config, 确保  485 # CONFIG_ARM_TRUSTZONE is not set  已经注释掉
     3 make

