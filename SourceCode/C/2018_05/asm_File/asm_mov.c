#include <stdio.h>

int main(int argc, char *argv)
{
	int a = 0;	
	int b = 0;

	//右移后，最低8可以放的下，否则会出错，
	//换句话说， 必须是连续在一起的8bit数据才是有效的
	__asm__ __volatile__(
	"mov r0, #0x1100 \n"	
	"mov %0, r0 \n"
	
	:"=r"(a)
	:
	:"r0","r1"
	);

	printf("%#x\n", a);

	return 0;
}
