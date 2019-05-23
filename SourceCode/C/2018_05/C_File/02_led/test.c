
#include "register.h"

//program:点亮led
//date:2018.05.29

int (*printf)(const char *format, ...) = (void *) (0xc3e11ad0);

//Brief:init led
//[In]:NA
//[Out]:NA
//Ret:always return 0
int led_init(void);

//Brief:led on
//[In]:led_num  [1, 4]
//[Out]:NA
//Ret: 0 success,  < 0 failed
int led_on(int led_num);

//Brief:led off
//[In]:led_num [1, 4]
//[Out]:NA
//Ret:0 success, < 0 failed
int led_off(int led_num);

void delay(int time);

int _start(void)
{
	int i = 1;
	printf("led test\n");

	led_init();

	while(1)
	{
		for (i = 1; i < 5; i++)
		{
			led_on(i);
			delay(0x7FFFF);
			led_off(i);
			delay(0x7FFFF);
		}
	}

	printf("led on\n");


	return 0;
}

//Brief:init led
//[In]:NA
//[Out]:NA
//Ret:always return 0
int led_init(void)
{
	//set to output mode
	GPM4CON &= ~0xFFFF;
	GPM4CON |= 0x1111;

	//set to 1 means turn off led
	GPM4DAT |= 0xF;

	return 0;
}

//Brief:led on
//[In]:led_num  [1, 4]
//[Out]:NA
//Ret: 0 success,  < 0 failed
int led_off(int led_num)
{
	if (led_num < 1 || led_num > 4)
	{
		return -1;
	}

	GPM4DAT |= 0x1 << (led_num - 1);

	return 0;
}

//Brief:led off
//[In]:led_num [1, 4]
//[Out]:NA
//Ret:0 success, < 0 failed
int led_on(int led_num)
{
	if (led_num < 1 || led_num > 4)
	{
		return -1;
	}

	GPM4DAT &= ~(0x1 << (led_num -1));

	return 0;
}

void delay(int time)
{
	while(time > 0)
	{
		time --;
	}
}
