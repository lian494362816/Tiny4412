#include <stdio.h>

//program:测试cp register
//date:2018.05.22

int main(int argc, char *argv[])
{
	int result = 0;

	__asm__ __volatile__(

	"ldr r0, =next \n"
	"mov r1, #0x11 \n"
	"mov pc, r0 \n"
	"mov r1, #0x22 \n"
	"next: \n"
	"mov %0, r1 \n"
			
	:"=r"(result)
	:
	:"r0", "r1"
	);

	printf("result:%#x\n", result);

	return 0;
}
