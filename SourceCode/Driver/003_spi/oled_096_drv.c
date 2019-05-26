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
#include <linux/spi/spi.h>
#include <linux/delay.h>
#include "my_error.h"
#include "my_s3c4412.h"
#include "oled_096.h"


#define OLED_MODULE_NAME "oled096"
#define OLED_CLASS_NAME "oled_class"


static struct class *oled_class= NULL;
static unsigned int oled_major = 0;
static int oled_index = 0;

static int oled_open(struct inode *inode, struct file *file)
{

    return 0;
}

static int oled_release(struct inode *inode, struct file *file)
{

    return 0;
}

ssize_t oled_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{

    return count;
}

ssize_t oled_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{

    return count;
}


long oled_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{

    return 0;
}

static struct file_operations oled_fops = {
    .open    = oled_open,
    .release = oled_release,
    .read    = oled_read,
    .write   = oled_write,
    .unlocked_ioctl = oled_ioctl,
};

static int oled_probe(struct spi_device *spi)
{
    struct oled_t *oled = NULL;
    int ret = -1;
    dev_t dev;

    oled = oled_alloc();
    if (!oled)
    {
        PRINT_FUNC_ERR(kmalloc);
        return -ENOMEM;
    }

    if (oled_major)
    {
        dev = MKDEV(oled_major, oled_index);
        ret = register_chrdev_region(dev, 1, OLED_MODULE_NAME);
        if (ret)
        {
            PRINT_FUNC_ERR(register_chrdev_region);
            goto err_free_oled;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&dev, oled_index, 1, OLED_MODULE_NAME);
        if (ret)
        {
            PRINT_FUNC_ERR(alloc_chrdev_region);
            goto err_free_oled;
        }
        oled_major = MAJOR(dev);
        oled->major = oled_major;
    }

    cdev_init(&oled->cdev, &oled_fops);

    ret = cdev_add(&oled->cdev, dev, 1);
    if (ret)
    {
        PRINT_FUNC_ERR(cdev_add);
        goto err_free_chr_region;
    }

    oled->device = device_create(oled_class, NULL, dev, NULL, "oled096_%d", oled_index);
    if (!oled->device)
    {
        PRINT_FUNC_ERR(device_create);
        goto err_cdev_del;
    }

    spi->dev.platform_data = oled;
    oled_index ++;
    oled->spi = spi;

    oled_hw_init(oled);

    //oled_show_char(oled, 0, 0, 'A');
    //oled_show_string(oled, 0, 0, "Tiny4412");
    oled_show_string(oled, 0, 0, "Tiny4412");

    /* 嵌入式太难了 */
    oled_show_chinese(oled, 0,4,0);
    oled_show_chinese(oled, 18,4,1);
    oled_show_chinese(oled, 36,4,2);
    oled_show_chinese(oled, 54,4,3);
    oled_show_chinese(oled, 72,4,4);
    oled_show_chinese(oled, 90,4,5);

    return 0;

err_cdev_del:
    cdev_del(&oled->cdev);

err_free_chr_region:
    unregister_chrdev_region(dev, 1);

err_free_oled:
    oled_free(oled);

    return ret;
}

static int oled_remove(struct spi_device *spi)
{
    struct oled_t *oled = spi->dev.platform_data;

    dev_t dev = MKDEV(oled_major, --oled_index);

    oled_hw_deinit(oled);

    device_destroy(oled_class, dev);

    cdev_del(&oled->cdev);

    unregister_chrdev_region(dev, 1);

    oled_free(oled);

    return 0;
}

const struct spi_device_id oled_id_table [] ={
    {"oled096", 0},
    { }, //END
};

static struct spi_driver oled_driver = {
    .driver = {
        .name = "oled096",
        .owner = THIS_MODULE,
    },
    .probe = oled_probe,
    .remove = oled_remove,
    .id_table = oled_id_table,
};

static int __init oled_init(void)
{
    int ret = -1;

    oled_class = class_create(THIS_MODULE, OLED_CLASS_NAME);
    if (IS_ERR(oled_class))
    {
        PRINT_FUNC_ERR("class create fail \n");
        ret = PTR_ERR(oled_class);
        return ret;
    }

    ret = spi_register_driver(&oled_driver);
    if (ret)
    {
        PRINT_FUNC_ERR("spi reg drv fail \n");
        goto err_free_class;
    }

    PRINT_INFO("%s init \n", OLED_MODULE_NAME);

    return 0;

err_free_class:
    class_destroy(oled_class);

    return ret;
}

static void __exit oled_exit(void)
{
    spi_unregister_driver(&oled_driver);

    class_destroy(oled_class);

    PRINT_INFO("%s exit \n", OLED_MODULE_NAME);
}

module_init(oled_init);
module_exit(oled_exit);

MODULE_LICENSE("GPL");
