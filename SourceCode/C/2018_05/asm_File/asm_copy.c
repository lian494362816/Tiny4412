#include <stdio.h>

//program:复制字符串
//date:2018.05.23

int main(int argc, char *argv[])
{
	char *p = "123 456  789";	
	char buff[20] = {0};
	int sum = 0;

	__asm__ __volatile__ (
			
	"mov r0, %1 \n"
	"mov r3, %2 \n"
	"mov r1, #0 \n"
	"loop: \n"
	"ldr r2, [r0] \n"
	"ldr r4, [r3] \n"
	"cmp r2, #0 \n"
	"strne r2, [r3] \n"
	"addne r1, #1 \n"
	"addne r0, #1 \n"
	"addne r3, #1 \n"
	"bne loop \n"
	"mov %0, r1 \n"	

	:"=r"(sum)
	:"r"(p),"r"(buff)
	:"r0", "r3","r1", "r2", "r4"
	);

	printf("sum:%d\n", sum);
	printf("buff:%s\n", buff);
	
	return 0;
}
