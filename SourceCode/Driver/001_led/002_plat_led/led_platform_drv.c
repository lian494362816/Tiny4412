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
#include <linux/slab.h>

#include "my_error.h"
#include "my_s3c4412.h"


#define LED_MODULE_NAME "s3c4412_led"
#define LED_MODULE_CLASS_NAME "s3c4412_led_class"

struct led_dev {
    struct s3c4412_gpio *gpio;
    int pin;
    int major;
    struct cdev cdev;
    struct device *device;
};

static struct class *led_class = NULL;
static unsigned int led_major = 0;


static int s3c4412_led_open(struct inode *inode, struct file *file)
{
    struct led_dev *led_dev = container_of(inode->i_cdev, struct led_dev, cdev);

    file->private_data = led_dev;

    return 0;
}

static int s3c4412_led_release(struct inode *inode, struct file *file)
{

    return 0;
}

static long s3c4412_led_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct led_dev *led_dev = file->private_data;

    if (_IOC_TYPE(cmd) != S3C4412_LED_MAGIC)
    {
        return -ENOTTY;
    }

    switch(cmd)
    {
        case S3C4412_LED_ON:
            //PRINT_INFO("led_dev->pin:%d \n", led_dev->pin);
            led_dev->gpio->dat &= ~(0x1 << led_dev->pin);
            break;

        case S3C4412_LED_OFF:
            led_dev->gpio->dat |= (0x1 << led_dev->pin);
            break;

        default:
            PRINT_ERR("cmd not find \n");
            break;
    }

    return 0;
}


static struct file_operations led_fops = {
    .open = s3c4412_led_open,
    .release = s3c4412_led_release,
    .unlocked_ioctl = s3c4412_led_ioctl,
    .owner =THIS_MODULE,
};

static int s3c4412_led_probe(struct platform_device *pdev)
{
    struct resource *res = NULL;
    dev_t dev;
    int ret = -1;
    unsigned int pin = *(int *)pdev->dev.platform_data;
    struct led_dev *led_dev = NULL;

    led_dev = kmalloc(sizeof(struct led_dev), GFP_KERNEL);
    if (!led_dev)
    {
        PRINT_ERR("kmalloc fail \n");
        return -ENOMEM;
    }
    led_dev->pin = pin;

    if (led_major)
    {
        dev = MKDEV(led_major, pdev->id);
        ret = register_chrdev_region(dev, 1, LED_MODULE_NAME);
        if (ret)
        {
            PRINT_ERR("reg chr region fail \n");
            goto err_free_led_dev;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&dev, pdev->id, 1, LED_MODULE_NAME);
        if (ret)
        {
            PRINT_ERR("alloc chr region fail \n");
            goto err_free_led_dev;
        }
        led_major = MAJOR(dev);
    }
    led_dev->major = led_major;

    cdev_init(&led_dev->cdev, &led_fops);
    led_dev->cdev.owner = THIS_MODULE;

    ret = cdev_add(&led_dev->cdev, dev, 1);
    if (ret)
    {
        PRINT_ERR("cdev add fail \n");
        goto err_free_chrdev;
    }

    led_dev->device = device_create(led_class, NULL, dev, NULL, "led%d", pdev->id);
    if (IS_ERR(led_dev->device))
    {
        PRINT_ERR("dev create fail \n");
        ret = PTR_ERR(led_dev->device);
        goto err_free_cdev;
    }

    platform_set_drvdata(pdev, led_dev);

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res)
    {
        PRINT_ERR("get res fail \n");
        ret = -ENOENT;
        goto err_free_device;
    }

    //PRINT_INFO("start:0x:%x \n", res->start);
    led_dev->gpio = ioremap(res->start, sizeof(struct s3c4412_gpio));
    if (!led_dev->gpio)
    {
        PRINT_ERR("ioremap fail \n");
        ret = -EBUSY;
        goto err_free_device;
    }

    //PRINT_INFO("pin:%d \n", pin);
    // set pin to output mode
    led_dev->gpio->con &= ~(0xF << (pin * 4));
    led_dev->gpio->con |= (0x1 << (pin * 4));

    // turn ff led
    led_dev->gpio->dat |= (0x1 << pin);

    PRINT_INFO("%s probe \n", res->name);

    return 0;

err_free_device:
    device_destroy(led_class, dev);

err_free_cdev:
    cdev_del(&led_dev->cdev);

err_free_chrdev:
    unregister_chrdev_region(dev, 1);

err_free_led_dev:
    kfree(led_dev);

    return ret;
}

static int s3c4412_led_remove(struct platform_device *pdev)
{
    struct led_dev *led_dev = platform_get_drvdata(pdev);
    int pin = led_dev->pin;
    dev_t dev = MKDEV(led_dev->major, pdev->id);

    device_destroy(led_class, dev);

    cdev_del(&led_dev->cdev);

    unregister_chrdev_region(dev, 1);

    //turn off
    led_dev->gpio->dat |= (0x1 << pin);
    iounmap(led_dev->gpio);
    kfree(led_dev);

    //PRINT_INFO("%s remove \n", pdev->dev.name);

    return 0;
}

static struct platform_driver s3c4412_led_driver = {
    .probe  = s3c4412_led_probe,
    .remove = s3c4412_led_remove,
    .driver = {
        .name = "s3c4412_led",
        .owner = THIS_MODULE,
    },
};

static int s3c4412_led_init(void)
{
    int ret = 0;

    led_major = 0;

    led_class = class_create(THIS_MODULE, LED_MODULE_CLASS_NAME);

    if (IS_ERR(led_class))
    {
        PRINT_ERR("class create fail \n");
        return PTR_ERR(led_class);
    }

    ret = platform_driver_register(&s3c4412_led_driver);
    if (ret)
    {
        PRINT_ERR("plat driver reg fail \n");
        goto err_free_class;
    }
    PRINT_INFO("%s init \n", LED_MODULE_NAME);

    return 0;

err_free_class:
    class_destroy(led_class);

    return ret;
}

static void s3c4412_led_exit(void)
{
    platform_driver_unregister(&s3c4412_led_driver);

    class_destroy(led_class);

    PRINT_INFO("%s exit \n", LED_MODULE_NAME);
}

module_init(s3c4412_led_init);
module_exit(s3c4412_led_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("LCC <494362816@qq.com>");
MODULE_DESCRIPTION("A simple character device driver for LEDs on Tiny4412 board");
