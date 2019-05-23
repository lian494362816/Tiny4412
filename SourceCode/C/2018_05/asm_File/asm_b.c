#include <stdio.h>

//program:测试b 跳转指令
//date:2018.05.21



int main(int argc, char *argv[])
{
	int result = 0;

	__asm__ __volatile__(
			
	"ldr r0, =0x11 \n"				
	"b next \n"
	"ldr r0, =0x22 \n"
	"next: \n"
	"mov %0, r0 \n"

	:"=r"(result)
	:
	:"r0"
			
	);

	printf("%#x\n", result);

	return 0;
}
