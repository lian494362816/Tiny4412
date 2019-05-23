#include "basedevice.h"

int (*printf)(const char *format, ...) = (void *) (0xc3e11ad0);

int _start(void)
{
	printf("hello \n");

	printf("%d, %d\n", 5 / 2, 5 % 3);
	
	return 0;
}
