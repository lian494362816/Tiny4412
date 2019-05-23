#include <stdio.h>

//program: 测试汇编中的 & | ~
//date:2018.05.20

int main(int argc, char *argv[])
{
	int a = 0;
	int b = 0;
	int c = 0;

	//& and
	//| orr
	//~ mvn
	
	__asm__ __volatile__(
	"ldr r0, =0xFF \n"			
	"ldr r1, =0xF \n"
	"ldr r2, =0xF0 \n"
	"and r0, #0xF  \n"
	"orr r1, #0xF0  \n"
	"mvn r2, r2 \n"
	"mov %0, r0 \n"
	"mov %1, r1 \n"
	"mov %2, r2 \n"


	:"=r"(a), "=r"(b), "=r"(c)
	:
	:"r0", "r1", "r2"
	);

	printf("a:%#x, b:%#x, c:%#x\n", a, b, c);


	return 0;
}
