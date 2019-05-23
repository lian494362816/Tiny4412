#ifndef __DEVICEBASE_H__
#define __DEVICEBASE_H__

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

#endif
