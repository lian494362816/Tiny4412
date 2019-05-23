
#include "reg.h"

int (*printf)(const char *format, ...) = (void *) (0xc3e11ad0);

//program:led key buzzer test
//date:2018.05.31

//Brief:init key
//[In]:NA
//[Out]:NA
//Ret:0
int key_init(void);

//Brief:get key status
//[In]:NA
//[Out]:NA
//Ret: 1110-> key1 triggle
//     1101-> key2 triggle
//     1011-> key3 triggle
//     0111-> key4 triggle
unsigned key_get_status(void);

//Brief:init led
//[In]:NA
//[Out]:NA
//Ret:0
int led_init(void);

//Brief:turn on led
//[In]:led_num [1,4]
//[Out]:NA
//Ret:0 success,  < 0 failed
int led_on(int led_num);

//Brief:turn off led
//[In]:led_num [1,4]
//[Out]:NA
//Ret:0 success, < 0 failed
int led_off(int led_num);

//Brief:init buzzer
//[In]:NA
//[Out]:NA
//Ret:0
int buzzer_init(void);

//Brief:buzzer rang
//[In]:NA
//[Out]:NA
//Ret:0
int buzzer_on(void);

//Brief:turn off buzzer
//[In]:NA
//[Out]:NA
//Ret:0
int buzzer_off(void);

//Brief:delay 
//[In]:time to sub
//[Out]:NA
//Ret:NA
void delay(unsigned int time);

int _start(void)
{
	unsigned int key_num = 0;
	//unsigned int key_array[4] = {0x1110, 0x1101, 0x1011, 0x0111};
	unsigned int key_array[4] = {14, 13, 11, 7};
	int i = 0;
	printf("key led buzzer program\n");	

	key_init();
	led_init();
	buzzer_init();

	while(1)
	{
		key_num = key_get_status();	
		for (i = 0; i < 4; i++)
		{
			if (key_num == key_array[i])
			{
				printf("key:%d \n", i+1);
				led_on(i+1);
				buzzer_on();
			}
		}

		delay(0x7FFFF);

		led_off(1);
		led_off(2);
		led_off(3);
		led_off(4);
		buzzer_off();
	}

	return 0;
}


int key_init(void)
{
	//GPX3_2 GPX3_4 GPX3_5 GPX4_6
	GPX3CON &= ~(0xF << 8);
}

unsigned int key_get_status(void)
{
	return ((GPX3DAT & (0xF << 2)) >> 2);
}

int led_init(void)
{
	GPM4CON &= ~(0xFFFF);
	GPM4CON |= 0x1111;
	GPM4DAT |= 0xF;

	return 0;
}

int led_on(int led_num)
{
	if (led_num < 1 || led_num > 4)
	{
		printf("led_num out of range:%d\n", led_num);
		return -1;
	}

	GPM4DAT &= ~(0x1 << (led_num -1));

	return 0;
}

int led_off(int led_num)
{
	if (led_num < 1 || led_num > 4)
	{
		printf("led_num out of range:%d\n", led_num);
		return -1;
	}

	GPM4DAT |= (0x1 << (led_num -1));

	return 0;

}
int buzzer_init(void)
{
	GPD0CON &= ~0xF;	
	GPD0CON |= 0x1;	
	GPD0DAT &= ~0x1;

	return 0;
}

int buzzer_on(void)
{
	GPD0DAT |= 0x1;

	return 0;
}

int buzzer_off(void)
{
	GPD0DAT &= ~0x1;

	return 0;
}

void delay(unsigned int time)
{
	while(0 != time)
	{
		time --;
	}
}
