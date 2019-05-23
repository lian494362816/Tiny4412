
#include "reg.h"
#include "basedevice.h"


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
