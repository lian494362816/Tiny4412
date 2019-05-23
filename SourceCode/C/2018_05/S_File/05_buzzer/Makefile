
all:
	arm-linux-gcc -c test.S -o test.o	
	arm-linux-ld -Ttext=0x40008000 test.o -o test
	arm-linux-objcopy -O binary test test.bin

clean:
	rm test *.o *.bin
