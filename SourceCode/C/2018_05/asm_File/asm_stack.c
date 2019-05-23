#include <stdio.h>

//program:堆栈的使用，基本命令push pop
//date:2018.05.22


int main(int argc, char *argv[])
{
	int a = 0;
	int b = 0;
	int c = 0;

	__asm__ __volatile__(
	
	"mov r0, #11 \n"
	"mov r1, #22 \n"
	"mov r2, #33 \n"

	"push {r0} \n"
	"push {r1} \n"
	"push {r2} \n"

	"mov r0, #44 \n"
	"mov r1, #55 \n"
	"mov r2, #66 \n"

	"pop {r2} \n"
	"pop {r1} \n"
	"pop {r0} \n"

	"mov %0, r0 \n"
	"mov %1, r1 \n"
	"mov %2, r2 \n"
	
	:"=r"(a), "=r"(b), "=r"(c)
	:
	:"r0", "r1", "r2"
	);

	printf("a:%d, b:%d, c:%d\n", a, b, c);


	return 0;
}
