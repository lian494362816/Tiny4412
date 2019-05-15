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
#include <linux/leds.h>
#include <linux/slab.h>
#include "my_error.h"
#include "my_s3c4412.h"

#define LED_MODULE_NAME "s3c4412_led"

LIST_HEAD(led_list);

typedef struct s3c4412_led {
    struct led_classdev led_cdev;
    s3c4412_gpio_t *gpio;
    int pin;
    struct list_head head;
}s3c4412_led_t;

static void s3c4412_led_set(struct led_classdev *led_cdev, enum led_brightness brightness)
{
    struct s3c4412_led *led;


    list_for_each_entry(led, &led_list, head)
    {
        if (!strcmp(led->led_cdev.name, led_cdev->name))
        {
            //PRINT_INFO("name:%s \n", led_cdev->name);
            if (LED_OFF == brightness)
            {
                led->gpio->dat |= (0x1 << led->pin);
            }
            else
            {
                led->gpio->dat &= ~(0x1 << led->pin);
            }
        }
    }

}

/*
    GPM4_0 -->LED1
    GMP4_1 -->LED2
    GPM4_2 -->LED3
    GPM4_3 -->LED4
    Hight level, led off
    Low   level, led on
*/

//led_classdev_register


static int s3c4412_led_probe(struct platform_device *pdev)
{
    struct s3c4412_led *led = NULL;
    struct resource *res = NULL;
    int pin = *(int *)pdev->dev.platform_data;
    int ret = -1;

    //PRINT_INFO("probe:%s \n", pdev->resource->name);

    led = kmalloc(sizeof(struct s3c4412_led), GFP_KERNEL);
    if (!led)
    {
        PRINT_ERR("kmalloc fail \n");
        return -ENOMEM;
    }
    memset(led, 0, sizeof(struct s3c4412_led));

    led->led_cdev.name = pdev->resource->name;
    led->led_cdev.brightness_set = s3c4412_led_set;

    res = platform_get_resource(pdev , IORESOURCE_MEM, 0);
    if (!res)
    {
        PRINT_ERR("get res fail \n");
        ret = -ENXIO;
        goto err_free_led;
    }

    led->gpio = ioremap(res->start, sizeof(struct s3c4412_gpio));
    if (!led->gpio)
    {
        PRINT_ERR("ioremap fail \n");
        ret = -ENOMEM;
        goto err_free_led;
    }

    led->pin = pin;
    //PRINT_INFO("pin:%d \n", pin);

    // set to output mode
    led->gpio->con &= ~(0xF << (pin * 4));
    led->gpio->con |= (0x1 << (pin * 4));

    // turn ff led
    led->gpio->dat |= (0x1 << pin);

    list_add_tail(&led->head, &led_list);

    ret = led_classdev_register(NULL, &led->led_cdev);
    if (ret)
    {
        PRINT_ERR("led class reg fail \n");
        goto err_free_io;
    }

    return 0;

err_free_io:
    iounmap(led->gpio);

err_free_led:
    kfree(led);
    led = NULL;

    return ret;
}

static int s3c4412_led_remove(struct platform_device *pdev)
{
    struct s3c4412_led *led = NULL;
    struct s3c4412_led *p = NULL;

    list_for_each_entry_safe(led, p, &led_list, head)
    {
        led_classdev_unregister(&led->led_cdev);
        iounmap(led->gpio);
        list_del(&led->head);
        kfree(led);
    }

    return 0;
}

static const struct platform_device_id s3c4412led_id_table [] = {
    {
        .name = "s3c4412_led",
    },

    { }, //end
};

static struct platform_driver s3c4412_led_driver = {
    .probe = s3c4412_led_probe,
    .remove = __devexit_p(s3c4412_led_remove),
    .id_table = s3c4412led_id_table,

    .driver = {
        .owner = THIS_MODULE,
        .name = "s3c4412_led",
    },
};

static int __init s3c4412_led_init(void)
{
    int ret = 0;

    ret = platform_driver_register(&s3c4412_led_driver);
    if (!ret)
    {
        PRINT_INFO("%s init \n", LED_MODULE_NAME);
    }

    return ret;
}

static void __exit s3c4412_led_exit(void)
{
    platform_driver_unregister(&s3c4412_led_driver);

    PRINT_INFO("%s exit \n", LED_MODULE_NAME);
}

module_init(s3c4412_led_init);
module_exit(s3c4412_led_exit);

MODULE_LICENSE("GPL");
