#include <stdio.h>

int main(int argc, char *argv)
{
	int a = 200;
	int b = 10;
	int c = 0;

	//%0  %1  %2
	//c   b   a
	//r2  r0  r1
	__asm__ __volatile__(
	
#if 0  //c=a+b
	"mov r0, %1 \n"	//r0->a=100
	"mov r1, %2 \n" //r1->b=200
	"add r2, r0, r1 \n"//r2=r0+r1
	"mov %0, r2 \n"//r2->c=210
#endif

#if 0
	//c=a-b
	"mov r0, %1 \n"
	"mov r1, %2 \n"
	"sub r2, r0, r1 \n"
	"mov %0, r2 \n"
#endif

	//c=a * b
#if 1
	"mov r0, %1 \n"
	"mov r1, %2 \n"
	"mul r2, r1, r0 \n"
	"mov %0, r2 \n"
#endif
			
	:"=r"(c)			
	:"r"(a), "r"(b)
	:"r0", "r1", "r2"
	);

	printf("a:%d, b:%d, c:%d\n", a, b, c);
	return 0;
}
