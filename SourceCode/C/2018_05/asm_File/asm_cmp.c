#include <stdio.h>

//program:测试汇编比较的命令
//date:2018.05.20


// ==  eq   qeual
// !=a nea  not qeual
// >   gt   grater than
// <   lt   less than
// >=  ge   grater equel
// <=  le   less equal

int main(int argc, char *argv[])
{
	int result = 0;

	__asm__ __volatile__ (

	"mov r0 ,#1 \n"
	"mov r1, #1 \n"
	"cmp r0, r1 \n"
	"moveq %0, #1 \n"
			
	:"=r"(result)
	:
	:"r0", "r1"
	);

	printf("result:%d\n", result);

	return 0;
}
