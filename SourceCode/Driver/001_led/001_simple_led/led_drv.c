#include <linux/device.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include "my_error.h"
#include "my_s3c4412.h"

#define LED_MODULE_NAME "s3c4412_led"
#define LED_MODULE_CLASS_NAME "s3c4412_led_class"
#define GPM4_BASE (0x11000000 + 0x02E0)

volatile struct s3c4412_gpio *led_gpio = NULL;

/*
    GPM4_0 -->LED1
    GMP4_1 -->LED2
    GPM4_2 -->LED3
    GPM4_3 -->LED4
    Hight level, led off
    Low   level, led on
*/

static int __init s3c4412_led_init(void)
{
    led_gpio = ioremap(GPM4_BASE, sizeof(struct s3c4412_gpio));
    if (!led_gpio)
    {
        PRINT_ERR("ioremap fail \n");
        return -ENOMEM;
    }

    //output mode
    led_gpio->con &= ~(0xFFFF);
    led_gpio->con |= 0x1111;

    //turn on
    led_gpio->dat &= ~(0xF);


    PRINT_INFO("%s init \n", LED_MODULE_NAME);

    return 0;
}

static void __exit s3c4412_led_exit(void)
{
    //turn off
    led_gpio->dat |= 0xF;

    iounmap(led_gpio);

    PRINT_INFO("%s exit \n", LED_MODULE_NAME);
}

module_init(s3c4412_led_init);
module_exit(s3c4412_led_exit);

MODULE_LICENSE("GPL");
