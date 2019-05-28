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
#include "oled_ioctl.h"


#define OLED_MODULE_NAME "oled096"
#define OLED_CLASS_NAME "oled_class"


static struct class *oled_class= NULL;
static unsigned int oled_major = 0;
static int oled_index = 0;

static int oled_open(struct inode *inode, struct file *file)
{
    struct oled_t *oled = container_of(inode->i_cdev, struct oled_t, cdev);

    file->private_data = oled;

    oled_hw_init(oled);

    //oled_show_string(oled, 0, 0, "hello");

    return 0;
}

static int oled_release(struct inode *inode, struct file *file)
{
    struct oled_t *oled = file->private_data;

    oled_hw_deinit(oled);

    return 0;
}

ssize_t oled_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    struct oled_t *oled = file->private_data;
    unsigned char data;

    if (1 == count)
    {
        if (copy_from_user(&data, buf, sizeof(data)))
        {
            PRINT_FUNC_ERR(copy_from_user);
            return -ENOMEM;
        }

        oled_write_byte(oled, data);
        return 1;
    }
    else if (count <= TMP_BUFF_SIZE)
    {
        if (copy_from_user(oled->tmp_buff, buf, count))
        {
            PRINT_FUNC_ERR(copy_from_user);
            return -ENOMEM;
        }

        oled_write_data(oled, oled->tmp_buff, count);

        return count;
    }
    else
    {
        PRINT_ERR("count to large:%d, max:%d \n", count, TMP_BUFF_SIZE);
        return -EINVAL;
    }

}

ssize_t oled_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{

    return -1;
}


long oled_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct oled_t *oled = file->private_data;
    struct oled_pos_t pos;
    unsigned char oled_cmd = 0;

    switch(cmd)
    {
        case OLED_ON:
            oled_display_on(oled);
            return 0;

        case OLED_OFF:
            oled_display_off(oled);
            return 0;

        case OLED_SET_POS:
            if (copy_from_user(&pos, (void __user *)arg, sizeof(struct oled_pos_t)))
            {
                PRINT_FUNC_ERR(copy_from_user);
                return -ENXIO;
            }

            return oled_set_pos(oled, pos.x, pos.y);

        case OLED_WRITE_CMD:
            if (copy_from_user(&oled_cmd, (void __user *)arg, sizeof(oled_cmd)))
            {
                PRINT_FUNC_ERR(copy_from_user);
                return -ENXIO;
            }

            return oled_write_cmd(oled, oled_cmd);

        default:
            PRINT_ERR("not cmd find \n");
            return -EINVAL;
    }

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

    oled_index ++;
    oled->spi = spi;
    spi_set_drvdata(spi, oled);

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
    struct oled_t *oled = (struct oled_t *)spi_get_drvdata(spi);

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
