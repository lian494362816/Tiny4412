
KERNEL_DIR=/home/black/Tiny4412/Kernel/linux-3.5

PWD=$(shell pwd)
EXTRA_CFLAGS +=-I$(PWD)/include

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) M=$(PWD) modules clean
	rm Module.symvers -f

obj-m +=  reg_editor_drv.o
