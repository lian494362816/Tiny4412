#include <stdio.h>

//program:测试arm汇编 位移命令
//date:2018.5.20

int main(int argc, char *argv[])
{
	int a = 0;
	int b = 0;
	int c = 0;

	//lsr logical shift right
	//lsl logical shift left
	//asr arithmetic shift right
	
	__asm__ __volatile__(
	"ldr r0, =0xFF \n"			
	"ldr r1, =0xFF \n"
	"ldr r2, =0x80000000 \n"
	"lsl r0, #4   \n"
	"lsr r1, #4   \n"
	"asr r2, #4   \n"
	"mov %0, r0 \n"	
	"mov %1, r1 \n"
	"mov %2, r2 \n"

	:"=r"(a), "=r"(b), "=r"(c)
	:
	:"r0","r1","r2"

	);

	printf("a:%#x, b:%#x, c:%#x\n", a, b, c);

	return 0;
}
