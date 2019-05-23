#include <stdio.h>

//program:计算字符串的长度
//date:2018.05.23

int main(int argc, char *argv[])
{
	char *p = "123 456  789";	
	int sum = 0;

	__asm__ __volatile__ (
			
	"mov r0, %1 \n"
	"mov r1, #0 \n"
	"loop: \n"
	"ldr r2, [r0] \n"
	"cmp r2, #0 \n"
	"addne r1, #1 \n"
	"addne r0, #1 \n"
	"bne loop \n"
	"mov %0, r1 \n"	

	:"=r"(sum)
	:"r"(p)
	:"r0", "r1", "r2"
	);

	printf("sum:%d\n", sum);
	
	return 0;
}
