
#include "keyreg.h"

int (*printf)(const char *format, ...) = (void *) (0xc3e11ad0);

//program:key test
//date:2018.05.30

//Brief: init key
//[In]:NA
//[Out]:NA
//Ret: always return 0
int key_init(void);

//Brief:get key stattus
//[In]:NA
//[Out]:NA
//Ret: 1110-> key1 triggle
//     1101-> key2 triggle
//     1011-> key3 triggle
//     0111-> key4 triggle
//     1111-> no key triggle
unsigned int key_get_status(void);

int _start(void)
{
	unsigned int key_num = 0;
	printf("key test\n");

	key_init();

	while(1)
	{
		key_num = key_get_status();

		if (0xF != key_num)
		{
			printf("%#x\n", key_num);
		}
	}

	return 0;
}

int key_init(void)
{
	GPX3CON &= ~(0xFFFF << 2);
	
	return 0;
}

unsigned int key_get_status(void)
{
	return ( (GPX3DAT & (0xF << 2)) >> 2);
}
