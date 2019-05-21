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
#include <linux/i2c.h>
#include "my_error.h"
#include "my_s3c4412.h"

#define EEPROM_MODULE_NAME "eeprom"
#define EEPROM_CLSSS_NAME "eeprom_class"
static struct class *eeprom_class = NULL;
static struct device *eeprom_device = NULL;
static struct i2c_client *eeprom_client = NULL;

static int eeprom_major = 0;

static int eeprom_open(struct inode *inode, struct file *file)
{

    return 0;
}

static int eeprom_relase(struct inode *inode, struct file *file)
{

    return 0;
}
//buf[0]: addr input
//buf[1]: value output
static ssize_t eeprom_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
    unsigned char addr = buf[0];
    int ret = -1;
    unsigned char value = 0;

#if 1
    //the api is ok
    ret = i2c_smbus_read_byte_data(eeprom_client, addr);
    if (ret < 0)
    {
        PRINT_ERR("i2c read fail \n");
        return value;
    }
#else

    struct i2c_msg msg[] = {
        {
            .addr = eeprom_client->addr,
            .flags = 0,
            .len = 1,
            .buf = &addr,
        },

        {
            .addr = eeprom_client->addr,
            .flags = I2C_M_RD,
            .len = 1,
            .buf = &value,
        },
    };
     // the api is also ok
    ret = i2c_transfer(eeprom_client->adapter, msg, 2);
    if (ret < 0)
    {
        PRINT_ERR("i2c trans fail \n");
        return ret;
    }
#endif
    value = (unsigned char) ret;
    //PRINT_WRN("addr:0x%x \n", addr);
    //PRINT_WRN("value:0x%x \n", value);

    ret = copy_to_user(&buf[1], &value, 1);
    if (ret)
    {
        PRINT_ERR("copy fail \n");
        return ret;
    }

    return 1;
}

static ssize_t eeprom_write(struct file *file ,const char __user *buf, size_t size, loff_t *ppos)
{
    int ret = 0;
    unsigned char addr = buf[0];
    unsigned char value = buf[1];

    if (addr > 0x80)
    {
        PRINT_ERR("addr to big:%d, max addr is:%d", addr, 0x80);
        return -EINVAL;
    }

    //PRINT_WRN("addr:0x%x \n", addr);
    //PRINT_WRN("value:0x%x \n", value);

    ret = i2c_smbus_write_byte_data(eeprom_client, addr, value);
    if (ret)
    {
        PRINT_ERR("i2c write fail \n");
        return ret;
    }

    return 2;
}

static struct file_operations eeprom_fops = {
    .open =    eeprom_open,
    .read =    eeprom_read,
    .write =   eeprom_write,
    .release = eeprom_relase,
};


static int eeprom_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    dev_t dev;
    struct cdev *cdev =  NULL;
    int ret = -1;

    cdev = kmalloc(sizeof(struct cdev), GFP_KERNEL);
    if (!cdev)
    {
        PRINT_ERR("kmalloc fail \n");
        return -ENOMEM;
    }

    if (eeprom_major)
    {
        dev = MKDEV(eeprom_major, 0);
        ret = register_chrdev_region(dev, 1, EEPROM_MODULE_NAME);
        if (ret)
        {
            PRINT_ERR("reg chrdev region fail \n");
            goto err_free_cdev;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&dev, 0, 1, EEPROM_MODULE_NAME);
        if (ret)
        {
            PRINT_ERR("alloc chrdev region fail \n");
            return -ENOMEM;
        }
        eeprom_major = MAJOR(dev);
    }

    cdev_init(cdev, &eeprom_fops);

    ret = cdev_add(cdev, dev, 1);
    if (ret)
    {
        PRINT_ERR("cdev add fail \n");
        goto err_free_chrdev_region;
    }

    eeprom_device = device_create(eeprom_class, NULL, dev, NULL, client->name);
    if (IS_ERR(eeprom_device))
    {
        PRINT_ERR("device create fial \n");
        ret = PTR_ERR(eeprom_device);
        goto err_free_del_cdev;
    }

    eeprom_client  = client;
    client->dev.platform_data = cdev;

    PRINT_INFO("client->name:%s \n", client->name);

    return 0;

err_free_del_cdev:
    cdev_del(cdev);

err_free_chrdev_region:
    unregister_chrdev_region(dev, 1);

err_free_cdev:
    kfree(cdev);

    return ret;
}

static int eeprom_remove(struct i2c_client *client)
{
    struct cdev *cdev = client->dev.platform_data;
    dev_t dev;

    dev = MKDEV(eeprom_major, 0);

    device_destroy(eeprom_class, dev);

    cdev_del(cdev);

    unregister_chrdev_region(dev, 1);

    kfree(cdev);

    return 0;
}

const struct i2c_device_id eeprom_id_table [] = {
    {"24aa025e48", 0},

    { }, //end
};

static  struct i2c_driver eeprom_driver = {
    .driver = {
        .name = EEPROM_MODULE_NAME,
    },

    .probe = eeprom_probe,
    .remove = __devexit_p(eeprom_remove),
    .id_table = eeprom_id_table,
};

static __init int eeprom_init(void)
{
    int ret = -1;
    eeprom_class = class_create(THIS_MODULE, EEPROM_CLSSS_NAME);
    if (IS_ERR(eeprom_class))
    {
        PRINT_ERR("class reate fail \n");
        ret = PTR_ERR(eeprom_class);
        return ret;
    }

    ret = i2c_add_driver(&eeprom_driver);
    if (ret)
    {
        PRINT_ERR("i2c add drv fail \n");
        goto err_free_class;
    }

    PRINT_INFO("%s init \n", EEPROM_MODULE_NAME);

    return 0;

err_free_class:
    class_destroy(eeprom_class);

    return ret;
}

static __exit void eeprom_exit(void)
{
    i2c_del_driver(&eeprom_driver);

    class_destroy(eeprom_class);

    PRINT_INFO("%s exit \n", EEPROM_MODULE_NAME);
}

module_init(eeprom_init);
module_exit(eeprom_exit);

MODULE_LICENSE("GPL");
