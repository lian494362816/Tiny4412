#include <stdio.h>

int main(int argc, char *argv[])
{
	int a = 0;

	__asm__ __volatile__ (
			

	//清除某些位，即置零某些位
	// 1111 1111   0xff
	// 0001 0000   (0x1 << 4)
	// 1110 1111   0xef
	
	"mov r0, #0xff \n"
	"mov r1, #(0x1<<4) \n"
	"bic r2, r0, r1 \n"
	"mov %0, r2 \n"

	:"=r"(a)
	:
	:"r0", "r1", "r2"
			
	);


	printf("a:%#x\n", a);
	
	return 0;
}
