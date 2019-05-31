#!/bin/bash

FILE_SYSTEM_DIR=/home/black/Tiny4412/filesystem
BUSYBOX_DIR=/home/black/Tiny4412/Busybox/busybox-1.30.0
ARM_LINUX_LIB_PATH=/opt/FriendlyARM/toolschain/4.5.1/arm-none-linux-gnueabi/lib
# ARM_LINUX_USR_LIB_PATH=/opt/arm-linux-4.3.2/arm-none-linux-gnueabi/libc/usr/lib

# 0 create file
rm ${FILE_SYSTEM_DIR} -rf
mkdir ${FILE_SYSTEM_DIR}

# 1 Use busybox install file system
cd ${BUSYBOX_DIR}
make CONFIG_PREFIX=${FILE_SYSTEM_DIR} install

# 2.1 Create dev
cd ${FILE_SYSTEM_DIR}
mkdir dev
# 2.2 Create console and null
cd dev
sudo mknod console c 5 1
sudo mknod null c 1 3

# 3.1 Create etc
cd ${FILE_SYSTEM_DIR}
mkdir etc
# 3.2 Cate inittab
cd etc
touch inittab
echo "console::askfirst:-/bin/sh" > inittab
echo "::sysinit:/etc/init.d/rcS" >> inittab
# 3.4 Create init.d
mkdir init.d

# 3.5 Create rcS
cd init.d
touch  rcS

echo "mount -a" > rcS
echo "mkdir /dev/pts" >> rcS
echo "mount -t devpts devpts /dev/pts" >> rcS
echo "echo /sbin/mdev > /proc/sys/kernel/hotplug" >> rcS
echo "mdev -s" >> rcS
echo "ifconfig eth0 192.168.1.80" >> rcS
echo "telnetd -p 23" >> rcS
chmod a+x rcS

# 3.6 Create fstab
cd ${FILE_SYSTEM_DIR}/etc
touch fstab

echo "#devicd         mount-point           type         options     dump    fsck-orfer" > fstab
echo "proc               /proc              proc         defaults    0            0" >> fstab
echo "sysfs              /sys               sysfs        defaults    0            0" >> fstab
echo "tmpfs              /dev               tmpfs        defaults    0            0" >> fstab
echo "devpts            /dev/pts            devpts       defaults    0            0" >> fstab
echo "debugfs           /sys/kernel/debug   debugfs      defaults    0            0" >> fstab

# 3.7 Create profile
cd ${FILE_SYSTEM_DIR}/etc
touch profile

echo "PS1='[root@4412 \w]#'" > profile

# 4.1 Create lib
cd ${FILE_SYSTEM_DIR}
mkdir lib
cd lib
cp ${ARM_LINUX_LIB_PATH}/*.so* -d ./

# 4.2 Create usr/lib
cd ${FILE_SYSTEM_DIR}
mkdir usr/lib -p
cd usr/lib
# cp ${ARM_LINUX_USR_LIB_PATH}/*.so* -d ./

# 4.3 Create lib/modules/3.4.2
cd ${FILE_SYSTEM_DIR}
mkdir lib/modules/3.4.2 -p

# 5.1 Config telnet
cd ${FILE_SYSTEM_DIR}/dev
mkdir pts
cd pts
sudo mknod ptmx c 5 2

# 5.2 Config telnet account:root  passwd:123
cd ${FILE_SYSTEM_DIR}/etc
touch passwd group shadow
echo "root:x:0:0:root:/:bin/sh" > passwd
echo "root:x:0:root" > group
echo "root:\$1\$FWEOGnxn\$KFJzPY1BvXX21Htui48T4/:12179:0:99999:7:::" > shadow

# 6 Create sys tmp mnt
cd ${FILE_SYSTEM_DIR}
mkdir sys tmp mnt
mkdir proc/net/dev -p

