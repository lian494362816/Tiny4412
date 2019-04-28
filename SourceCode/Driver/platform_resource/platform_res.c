#include <linux/device.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <asm/uaccess.h>
#include <linux/types.h>

#define GPM4_BASE (0x11000000 + 0x02E0)

/*
    GPM4_0 ->led1
    GPM4_1 ->led2
    GPM4_2 ->led3
    GPM4_3 ->led4
*/
unsigned int led1_pin = 0;
unsigned int led2_pin = 1;
unsigned int led3_pin = 2;
unsigned int led4_pin = 3;

static void led_dev_release(struct device * pdev)
{
    //nothing
}

static struct resource led1_resources[] = {
    [0] = {
        .flags = IORESOURCE_MEM,
        .start = GPM4_BASE, //control register start address
        .end = GPM4_BASE + 4 - 1,  //can be any number
        .name = "led1",
    },
};

static struct platform_device led1 = {
    .name = "s3c4412_led",
    .id = 1, //decide minor and use to create "/dev/led%d"
    .resource = led1_resources,
    .num_resources = ARRAY_SIZE(led1_resources),
    .dev = {
        .release = led_dev_release,
        .platform_data = &led1_pin, //GPM4 hardware pin num
    },
};

static struct resource led2_resources[] = {
    [0] = {
        .flags = IORESOURCE_MEM,
        .start = GPM4_BASE,
        .end = GPM4_BASE + 4 -1,
        .name = "led2",
    }
};

static struct platform_device led2 = {
    .name = "s3c4412_led",
    .id = 2,
    .resource = led2_resources,
    .num_resources = ARRAY_SIZE(led2_resources),
    .dev = {
        .release = led_dev_release,
        .platform_data = &led2_pin,
    },
};

static struct resource led3_resources[] = {
    [0] = {
        .flags = IORESOURCE_MEM,
        .start = GPM4_BASE,
        .end = GPM4_BASE + 4 -1,
        .name = "led3",
    },
};

static struct platform_device led3 = {
    .name = "s3c4412_led",
    .id = 3,
    .resource = led3_resources,
    .num_resources = ARRAY_SIZE(led3_resources),
    .dev = {
        .release = led_dev_release,
        .platform_data = &led3_pin,
    },
};


static struct resource led4_resources[] = {
    [0] = {
        .flags = IORESOURCE_MEM,
        .start = GPM4_BASE,
        .end = GPM4_BASE + 4 -1,
        .name = "led4",
    },
};

static struct platform_device led4 = {
    .name = "s3c4412_led",
    .id = 4,
    .resource = led4_resources,
    .num_resources = ARRAY_SIZE(led4_resources),
    .dev = {
        .release = led_dev_release,
        .platform_data = &led4_pin,
    },
};

static struct platform_device *led_devices[] = {
    &led1,
    &led2,
    &led3,
    &led4,
};

static int __init s3c4412_platform_res_init(void)
{
    platform_add_devices(led_devices, ARRAY_SIZE(led_devices));

    return 0;
}

static void __exit s3c4412_platform_res_exit(void)
{
    int i = 0;

    for (i = 0; i < ARRAY_SIZE(led_devices); i++)
    {
        platform_device_unregister(led_devices[i]);
    }
}

module_init(s3c4412_platform_res_init);
module_exit(s3c4412_platform_res_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LCC <494362816@qq.com>");
MODULE_DESCRIPTION("Resources On Tiny4412 Board");
