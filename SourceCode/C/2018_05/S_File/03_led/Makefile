
all:
	arm-linux-gcc -c test.S -o test.o
	arm-linux-ld -Ttext=0x4000800 test.o -o test
	arm-linux-objcopy -O binary test test.bin

clean:
	rm -r *.o test test.bin
