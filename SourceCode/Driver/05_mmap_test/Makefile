
#KERNEL_DIR=/home/black/Tiny4412/Kernel/linux-3.5
KERNEL_DIR=/home/black/Tiny4412/Kernel/linux-4.4

PWD=$(shell pwd)
EXTRA_CFLAGS +=-I$(PWD)/include

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) M=$(PWD) modules clean
	rm Module.symvers -f

.PHONY: modules  clean

obj-m += mmap_test_drv.o
