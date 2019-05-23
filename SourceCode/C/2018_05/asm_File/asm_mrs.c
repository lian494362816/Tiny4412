#include <stdio.h>

//program: 测试mrs命令，查看cmp命令对cpsr register的影响
//date: 2018.05.21


void print_bit(unsigned int u32data)
{
	int i = 0;

	for (i = 0; i < 32; i++)
	{
		printf("%d",( (u32data & (0x80000000 >> i)) ? 1 : 0));
	}

	printf("\n");

}

int main(int argc, char *argv[])
{
	unsigned int cpsr_before = 0;
	unsigned int cpsr_after = 0; 

	__asm__ __volatile__(
			
	"mrs r0, cpsr \n"	
	"mov %0, r0 \n"
	"mov r1, #2 \n"
	"mov r2, #1 \n"
	"cmp r1, r2 \n"
	"mrs r0, cpsr \n"
	"mov %1, r0 \n"
	
	:"=r"(cpsr_before), "=r"(cpsr_after)
	:
	:"r0", "r1", "r2"
	);

	print_bit(cpsr_before);
	print_bit(cpsr_after);

	return 0;
}
