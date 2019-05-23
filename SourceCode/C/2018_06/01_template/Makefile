
all:
	arm-linux-gcc -c test.c -o test.o -fno-builtin
	arm-linux-gcc -c basedevice.c -o basedevic.o -fno-builtin
	arm-linux-gcc -c string.c -o string.o -fno-builtin
	arm-linux-ld -Ttest.lds *.o -o test
	arm-linux-objcopy -O binary test test.bin
clean:
	rm test *.bin *.o
	

