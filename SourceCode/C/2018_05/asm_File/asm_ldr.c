#include <stdio.h>


int main(int argc, char *argv[])
{
	int a = 0;	


	//一般大于 0xFF 的数就使用ldr
	__asm__ __volatile__(
	"ldr r0, =0xFFFFFFFF \n"			
	"mov %0, r0 \n"

	:"=r"(a)
	:
	:"r0"

	);

	printf("%#x\n", a);

	return 0;
}
