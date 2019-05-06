#!/bin/bash

RAM_DISK_NAME=ramdisk
# ramdisk size = COUNT * 1KB
COUNT=16384
TMP_FILE=_tmp_file
# 使用busybox 制作的文件系统
ROOTFS=/home/black/Tiny4412/filesystem

rm -rf ${RAM_DISK_NAME}
 
sudo dd if=/dev/zero of=${RAM_DISK_NAME} bs=1k count=${COUNT}

sudo mkfs.ext4 -F ${RAM_DISK_NAME}

sudo mkdir -p ${TMP_FILE}
sudo mount -t ext4 ${RAM_DISK_NAME} ${TMP_FILE}

sudo cp ${ROOTFS}/* ${TMP_FILE}  -raf

sudo umount ${TMP_FILE}
 
sudo gzip --best -c ${RAM_DISK_NAME} > ${RAM_DISK_NAME}.gz
 
sudo mkimage -n "ramdisk" -A arm -O linux -T ${RAM_DISK_NAME} -C gzip -d ${RAM_DISK_NAME}.gz ${RAM_DISK_NAME}.img

rm ${TMP_FILE} -rf
rm ${RAM_DISK_NAME} -rf
rm ${RAM_DISK_NAME}.gz -rf
