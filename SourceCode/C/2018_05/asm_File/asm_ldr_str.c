#include <stdio.h>

//program:测试ldr str当做操作地址
//date:2018.05.23



int main(int argc, char *argv[])
{
	int sum = 100;	

	__asm__ __volatile__ (
			
	"mov r0, %0 \n"			
	"ldr r1, [r0] \n" //*r1 = r0 将r0当做地址，并把地址的值给r1
	"add r1, #100 \n" //*r1 += 100
	"str r1, [r0] \n"//*r0 = r1
			
	:
	:"r"(&sum)
	:"r0", "r1"

	);
	printf("sum:%d\n", sum);

	return 0;
}
