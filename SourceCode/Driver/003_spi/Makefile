
KERNEL_DIR=/home/black/Tiny4412/Kernel/linux-3.5

PWD=$(shell pwd)
EXTRA_CFLAGS +=-I$(PWD)/include

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	# make -C $(KERNEL_DIR) M=$(PWD) modules clean
	# rm Module.symvers -f
	rm *.o *.ko modules.order  Module.symvers  *.mod.c -f

obj-m +=  oled.o
oled-objs += oled_096.o oled_096_drv.o
