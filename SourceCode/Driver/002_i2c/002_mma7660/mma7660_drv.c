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
#include <linux/mma7660.h>
#include "my_error.h"
#include "my_s3c4412.h"


#define MMA7660_MODULE_NAME "mma7660"
#define MMA7660_CLASS_NAME "mma7660_class"

static struct class *mma7660_class = NULL;
static struct device *mma7660_device = NULL;
static int mma7660_major = 0;
static struct i2c_client *mma7660_i2c_client = NULL;

#define MMA7660_ALERT    (0x1 << 6)
#define MMA7660_NEGATIVE (0x1 << 5)

#define __mma7660_need_retry(x) ((x) & (0x1 << 6))
#define __need_retry(__v)	(__v & (1 << 6))

#define __mma7660_is_negative(x) ((x) & (0x1 << 5))
#define __mma7660_get_value(x) ((x) & 0x1F)

static ssize_t mma7660_show_xyz_axis(struct device *d, struct device_attribute *attr, char *buf)
{
    int tmp_buf[3] = {0};
    int i = 0;

    for (i = 0; i < ARRAY_SIZE(tmp_buf); i++)
    {
        do{
            tmp_buf[i] = i2c_smbus_read_byte_data(mma7660_i2c_client, i);
            if (tmp_buf[i] < 0)
            {
                PRINT_ERR("i2c read fail \n");
                return -1;
            }
        }while(__mma7660_need_retry(tmp_buf[i]));

        tmp_buf[i] = __mma7660_is_negative(tmp_buf[i]) ? (-__mma7660_get_value(tmp_buf[i])) : __mma7660_get_value(tmp_buf[i]);
    }

    return sprintf(buf, "%3d, %3d, %3d\n", tmp_buf[0], tmp_buf[1], tmp_buf[2]);

}

static ssize_t mma7660_show_reg(struct device *dev, struct device_attribute *attr, char *buf)
{
    int len = 0;
    int reg = 0;
    int value = 0;
    int i = 0;

    for (reg = 0; reg <= MMA7660_PD; reg++)
    {
        //can't retry
        value = i2c_smbus_read_byte_data(mma7660_i2c_client, reg);
        if (value < 0)
        {
            PRINT_WRN("i2c read fail, reg = %d \n", reg);
        }

        len += sprintf(buf + len, "REG: [0x%02x] = 0x%02x ...... [ ", reg, value);
        for (i = 7; i >= 0; i--)
        {
            len += sprintf(buf + len, "%d", (value >> i) &0x1);
            if (0 == (i % 4) )
            {
                len += sprintf(buf + len, " ");
            }
        }
        len += sprintf(buf + len, "] \n");

    }

    return len;
}

static ssize_t mma7660_store_reg(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    int ret = 0;
    int value = 0;
    int reg = 0;
    int tmp_value = 0;

    ret = sscanf(buf, "%x %x", &reg, &value);
    if (2 == ret)
    {
        if (reg < 0 || reg > MMA7660_PD)
        {
            PRINT_ERR("reg out of area max 0x:%x \n", MMA7660_PD);
            goto err_out;
        }

        ret = i2c_smbus_write_byte_data(mma7660_i2c_client, reg, value);
        if (ret)
        {
            PRINT_ERR("i2c write fail \n");
            goto err_out;
        }

        tmp_value = i2c_smbus_read_byte_data(mma7660_i2c_client, reg);
        if (tmp_value < 0)
        {
            PRINT_WRN("i2c read fail, reg = %d \n", reg);
        }

        if (tmp_value == value)
        {
            printk("write [0x%x] = 0x%x OK\n", reg, value);
        }
        else
        {
             printk("write [0x%x] = 0x%x Error\n", reg, value);
             printk("ret   [0x%x] = 0x%x \n", reg, tmp_value);
        }
    }

err_out:
    return count;
}

static DEVICE_ATTR(xyz_axis, S_IRUGO, mma7660_show_xyz_axis, NULL);
static DEVICE_ATTR(reg, S_IRUGO | S_IWUGO, mma7660_show_reg, mma7660_store_reg);

static struct attribute* mma7660_attrs[] = {
    &dev_attr_xyz_axis.attr,
    &dev_attr_reg.attr,
    NULL,
};

static struct attribute_group mma7660_attr_gropu = {
    .name = NULL,
    .attrs = mma7660_attrs,
};


/*
#define MMA7660_XOUT			0x00	// 6-bit output value X
#define MMA7660_YOUT			0x01	// 6-bit output value Y
#define MMA7660_ZOUT			0x02	// 6-bit output value Z
#define MMA7660_TILT			0x03	// Tilt status
#define MMA7660_SRST			0x04	// Sampling Rate Status
#define MMA7660_SPCNT			0x05	// Sleep Count
#define MMA7660_INTSU			0x06	// Interrupt Setup
#define MMA7660_MODE			0x07	// Mode
#define MMA7660_SR				0x08	// Auto-Wake/Sleep and Debounce Filter
#define MMA7660_PDET			0x09	// Tap Detection
#define MMA7660_PD				0x0a	// Tap Debounce Count
*/

static int mma7660_hw_init(struct i2c_client *client)
{
    unsigned char value = 0;
    int i = 0;
    int ret = -1;
    unsigned char tmp_buff[3] = {0x11, 0x22, 0x33};

    //must placed in stand mode than change to other mode

    //Standby Mode
    i2c_smbus_write_byte_data(client, MMA7660_MODE, 0);

    //Test Mode
    i2c_smbus_write_byte_data(client, MMA7660_MODE, 4);

    //write to X Y Z and read it
    for (i = 0; i < 3; i++)
    {
        ret = i2c_smbus_write_byte_data(client, i, tmp_buff[i]);
        if (ret)
        {
            PRINT_ERR("i2c write fail \n");
            return ret;
        }

        value = i2c_smbus_read_byte_data(client, i);
        if (value != tmp_buff[i])
        {
            PRINT_ERR("read fail, expect 0x%x, return value 0x%x \n", tmp_buff[i], value);
            return -1;
        }

        //PRINT_INFO("expect 0x%x, return value 0x%x \n", tmp_buff[i], value);
    }

    i2c_smbus_write_byte_data(client, MMA7660_MODE, 0);

    //Active  Mode
    i2c_smbus_write_byte_data(client, MMA7660_MODE, 1);

    // Active and Auto-Sleep Mode1 Sameple 120Hz
    // Auto-Wake Mode Sample 64Hz
    // FILT 8 samples
    i2c_smbus_write_byte_data(client, MMA7660_SR, 0x7 << 5);

    //i2c_smbus_write_byte_data(client, MMA7660_INTSU, 0x1 << 4);
    //i2c_smbus_write_byte_data(client, MMA7660_INTSU, 0xe7);

    return 0;
}

static int mma7660_hw_deinit(struct i2c_client *client)
{
    //standby mode
    i2c_smbus_write_byte_data(client, MMA7660_MODE, 0);

    return 0;
}

static int mma7660_open(struct inode *inode, struct file *file)
{

    return 0;
}

static int mma7660_release(struct inode *inode, struct file *file)
{

    return 0;
}

static long mma7660_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{

    return 0;
}

ssize_t mma7660_write(struct file *file, const char __user *buf, size_t size, loff_t *ppos)
{

    return 0;
}

//buf[0] x
//buf[1] y
//buf[2] z
ssize_t mma7660_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
    int i = 0;
    int tmp_buf[3] = {0};
    int ret = -1;

    /* 需要使用int 来表示， 使用char 负数会有问题 */
    if (!buf)
    {
        PRINT_ERR("null poiner \n");
        return -EINVAL;
    }

    for (i = 0; i < ARRAY_SIZE(tmp_buf); i++)
    {
        do{
            tmp_buf[i] = i2c_smbus_read_byte_data(mma7660_i2c_client, i);
            if (tmp_buf[i] < 0)
            {
                PRINT_ERR("i2c read fail \n");
                return -1;
            }
        }while(__mma7660_need_retry(tmp_buf[i]));

        tmp_buf[i] = __mma7660_is_negative(tmp_buf[i]) ? (-__mma7660_get_value(tmp_buf[i])) : __mma7660_get_value(tmp_buf[i]);
        //PRINT_INFO("tmp_buf[%d]:%d \n", i, tmp_buf[i]);
    }

    ret = copy_to_user(buf, tmp_buf, sizeof(tmp_buf));

    return ret;
}

static struct file_operations mma7660_fops = {
    .open = mma7660_open,
    .release = mma7660_release,
    .unlocked_ioctl = mma7660_ioctl,
    .write = mma7660_write,
    .read = mma7660_read,
};

static int mma7660_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    dev_t dev;
    struct cdev *cdev = NULL;
    int ret = -1;
    struct i2c_adapter *adapter = to_i2c_adapter(client->dev.parent);

    ret = i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE | I2C_FUNC_SMBUS_BYTE_DATA);
    if (!ret)
    {
        PRINT_ERR("i2c check func fail \n");
        return -ENXIO;
    }

    mma7660_i2c_client = client;

    cdev = kmalloc(sizeof(struct cdev), GFP_KERNEL);
    if (!cdev)
    {
        PRINT_ERR("kmalloc fail \n");
        return -ENOMEM;
    }

    if (mma7660_major)
    {
        dev = MKDEV(mma7660_major, 0);
        ret = register_chrdev_region(dev, 1, MMA7660_MODULE_NAME);
        if (ret)
        {
            PRINT_ERR("reg chrdev region fail \n");
            goto err_free_cdev;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&dev, 0, 1, MMA7660_MODULE_NAME);
        if (ret)
        {
            PRINT_ERR("alloc chrdev region fail \n");
            goto err_free_cdev;
        }
        mma7660_major = MAJOR(dev);
    }

    cdev_init(cdev, &mma7660_fops);
    ret = cdev_add(cdev, dev, 1);
    if (ret)
    {
        PRINT_ERR("cdev add fail \n");
        goto err_free_chrdev_region;
    }

    mma7660_device = device_create(mma7660_class, NULL, dev, NULL, "mma7660");
    if (IS_ERR(mma7660_device))
    {
        PRINT_ERR("devcie create fail \n");
        ret = PTR_ERR(mma7660_device);
        goto err_del_cdev;
    }

    ret = sysfs_create_group(&mma7660_device->kobj, &mma7660_attr_gropu);
    if (ret)
    {
        PRINT_ERR("syfs create fail \n");
        goto err_destroy_dev;
    }

    client->dev.platform_data = cdev;

    mma7660_hw_init(client);

    return 0;
err_destroy_dev:
    device_destroy(mma7660_class, dev);

err_del_cdev:
    cdev_del(cdev);

err_free_chrdev_region:
    unregister_chrdev_region(dev, 1);

err_free_cdev:
    kfree(cdev);

    return ret;

}

static int mma7660_remove(struct i2c_client *client)
{
    dev_t dev = MKDEV(mma7660_major, 0);
    struct cdev *cdev = client->dev.platform_data;

    mma7660_hw_deinit(client);

    sysfs_remove_group(&mma7660_device->kobj,  &mma7660_attr_gropu);

    device_destroy(mma7660_class, dev);

    cdev_del(cdev);

    unregister_chrdev_region(dev, 1);

    kfree(cdev);

    return 0;
}

static int mma7660_suspend(struct i2c_client *client, pm_message_t mesg)
{

    return 0;
}

static int mma7660_resume(struct i2c_client *client)
{
    return 0;
}

struct i2c_device_id mma7660_id_table[] = {
    {"mma7660", 0},

    { }, //end
};


struct i2c_driver mma7660_driver = {
    .driver = {
        .name = MMA7660_MODULE_NAME,
    },

    .probe = mma7660_probe,
    .remove = __devexit_p(mma7660_remove),
    .suspend = mma7660_suspend,
    .resume = mma7660_resume,
    .id_table = mma7660_id_table,
};

static __init int mma7660_init(void)
{
    int ret = -1;

    mma7660_class = class_create(THIS_MODULE, MMA7660_CLASS_NAME);
    if (IS_ERR(mma7660_class))
    {
        PRINT_ERR("class create fail \n");
        ret = PTR_ERR(mma7660_class);
        return ret;
    }

    ret = i2c_add_driver(&mma7660_driver);
    if (ret)
    {
        PRINT_ERR("i2c add driver fail \n");
        goto err_free_class;
    }

    PRINT_INFO("%s init \n", MMA7660_MODULE_NAME);

    return 0;

err_free_class:
    class_destroy(mma7660_class);

    return ret;
}

static __exit void mma7660_exit(void)
{
    i2c_del_driver(&mma7660_driver);

    class_destroy(mma7660_class);

    PRINT_INFO("%s exit \n", MMA7660_MODULE_NAME);
}

module_init(mma7660_init);
module_exit(mma7660_exit);

MODULE_LICENSE("GPL");
