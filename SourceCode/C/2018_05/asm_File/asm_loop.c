#include <stdio.h>

//program:使用b instruction 来实现循环
//        简单的实现1加到100
//date:2018.05.21



int main(int argc, char *argv[])
{
	int sum = 0;

	__asm__ __volatile__(
			
	"mov r0, #0 \n"			
	"mov r1, #1 \n"
	
	"loop: \n"
	"add r0, r1 \n"
	"cmp r1, #100 \n"
	"add r1, #1 \n"
	"bne loop \n"
	"mov %0, r0 \n"

	:"=r"(sum)
	:
	:"r0", "r1"
	);


	printf("sum = %d\n", sum);

	return 0;
}
