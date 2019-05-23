#include <stdio.h>

//program:测试跳转指令, 类似与一个函数
//date:2018.5.22


__asm__(
".global T_Add \n"
"T_Add: \n"
"stmfd sp!, {lr} \n"

"add r0, r1 \n"

"ldmfd sp!, {lr} \n"

"bx lr  \n" //等于mov pc lr
		
);


int main(int argc, char *argv[])
{
	int result1 = 0;
	int result2 = 0;

	//通过保存lr register 来实现程序的跳转
	__asm__ __volatile__(
	"stmfd sp!, {lr} \n"	//store lr register
	
	"mov r0, #11 \n"
	"mov r1, #22 \n"

	"bl T_Add \n"

	"mov %0, r0 \n"
	"mov %1, r1 \n"

	"ldmfd sp!, {lr} \n" //load lr register

	:"=r"(result1), "=r"(result2)
	:
	:"r0", "r1"

	);

	printf("result1:%d, result2:%d\n", result1, result2);

	return 0;
}
