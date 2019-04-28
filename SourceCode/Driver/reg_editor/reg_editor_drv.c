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
#include "reg_editor.h"


#define REG_EDITOR_MODULE_NAME "reg_editor"
#define REG_EDITOR_MAJOR (0)
#define REG_EDITOR_CLASS_NAME "reg_editor_class"
#define REG_EDITOR_CLASS_DEVICE_NAME "reg_editor"
#define REG_EDITOR_CHRDEV_REGION (1)

static int reg_editor_major = 0;
static struct class *reg_editor_class = NULL;
static struct device *reg_editor_class_device = NULL;
static struct cdev reg_editor_cdev;
static struct reg_editor_t reg_editor_val;


static int reg_editor_open(struct inode *inode, struct file *file)
{

    return 0;
}

static int reg_editor_release(struct inode *inode, struct file *file)
{

    return 0;
}


static long reg_editor_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    volatile unsigned char *p8 = NULL;
    volatile unsigned short *p16 = NULL;
    volatile unsigned int *p32 = NULL;

    if (_IOC_TYPE(cmd) != REG_EDITOR_MAGIC)
    {
        return -ENOTTY;
    }

    if (copy_from_user(&reg_editor_val, (const void __user *)arg, sizeof(reg_editor)))
    {
        PRINT_ERR("copy from user fail \n");
        return -EFAULT;
    }

    p8 = (volatile unsigned char *)ioremap(reg_editor_val.addr, 4);
    if (!p8)
    {
        PRINT_ERR("ioremap fail \n");
        return -ENOMEM;
    }
    p16 = (volatile unsigned short *) p8;
    p32 = (volatile unsigned int *) p8;

    //PRINT_INFO("addr:%x \n", reg_editor_val.addr);
    //PRINT_INFO("val:%x \n",  reg_editor_val.val);

    switch(cmd)
    {
        case REG_EDITOR_R8:
            reg_editor_val.val = *p8;
            //PRINT_INFO("r8 \n");
            break;

        case REG_EDITOR_R16:
            reg_editor_val.val = *p16;
            //PRINT_INFO("r16 \n");
            break;

        case REG_EDITOR_R32:
            reg_editor_val.val = *p32;
            //PRINT_INFO("r32 \n");
            break;

        case REG_EDITOR_W8:
            *p8 = (unsigned char)reg_editor_val.val;
            reg_editor_val.val = *p8;
            //PRINT_INFO("w8 \n");
            break;

        case REG_EDITOR_W16:
            *p16 = (unsigned short)reg_editor_val.val;
            reg_editor_val.val = *p16;
            //PRINT_INFO("w16 \n");
            break;

        case REG_EDITOR_W32:
            *p32 = (unsigned int)reg_editor_val.val;
            reg_editor_val.val = *p32;
            //PRINT_INFO("w32 \n");
            break;

        default:
            PRINT_ERR("cmd error:%x \n", cmd);
            return -ENOTTY;

    }

    //if (cmd == REG_EDITOR_R8 || cmd == REG_EDITOR_R16 || cmd == REG_EDITOR_R32)
    {
        ret = copy_to_user((void __user *)arg, &reg_editor_val, sizeof(reg_editor));
        if (ret)
        {
            PRINT_ERR("copy to user fail \n");
            return -EIO;
        }
    }

    //PRINT_INFO("val:0x%x \n", reg_editor_val.val);

    iounmap(p8);

    return 0;
}

static struct file_operations reg_editor_fops = {

    .owner = THIS_MODULE,
    .open = reg_editor_open,
    .release = reg_editor_release,
    .unlocked_ioctl = reg_editor_ioctl,
};

static int reg_editor_init(void)
{
    int ret = 0;
    dev_t dev;

    if (reg_editor_major)
    {
        dev = MKDEV(reg_editor_major, 0);
        ret = register_chrdev_region(dev, REG_EDITOR_CHRDEV_REGION, REG_EDITOR_MODULE_NAME);
        if (ret)
        {
            PRINT_ERR("reg chrdev region fail \n");
            return -1;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&dev, 0, REG_EDITOR_CHRDEV_REGION, REG_EDITOR_MODULE_NAME);
        if (ret)
        {
            PRINT_ERR("alloc chrdev region fail \n");
            return -1;
        }
        reg_editor_major = MAJOR(dev);
    }

    cdev_init(&reg_editor_cdev, &reg_editor_fops);
    reg_editor_cdev.owner = THIS_MODULE;

    ret = cdev_add(&reg_editor_cdev, dev, REG_EDITOR_CHRDEV_REGION);
    if (ret)
    {
        PRINT_ERR("cdev add fail \n");
        return -1;
    }

    reg_editor_class = class_create(THIS_MODULE, REG_EDITOR_CLASS_NAME);
    if (!reg_editor_class)
    {
        PRINT_ERR("class create fail \n");
        goto err_free_chrdev;
    }

    reg_editor_class_device = device_create(reg_editor_class, NULL, dev, NULL, REG_EDITOR_CLASS_DEVICE_NAME);
    if (!reg_editor_class_device)
    {
        PRINT_ERR("class device create fail \n");
        goto err_free_class;
    }

    PRINT_INFO("%s init ok \n", REG_EDITOR_MODULE_NAME);

    return ret;

err_free_class:
    class_destroy(reg_editor_class);

err_free_chrdev:
    cdev_del(&reg_editor_cdev);

    unregister_chrdev_region(dev, REG_EDITOR_CHRDEV_REGION);

    return -1;
}

static void reg_editor_exit(void)
{
    dev_t dev;
    dev = MKDEV(reg_editor_major, 0);

    cdev_del(&reg_editor_cdev);

    unregister_chrdev_region(dev, REG_EDITOR_CHRDEV_REGION);

    device_destroy(reg_editor_class, dev);

    class_destroy(reg_editor_class);

    PRINT_INFO("%s exit \n", REG_EDITOR_MODULE_NAME);
}

module_init(reg_editor_init);
module_exit(reg_editor_exit);

MODULE_LICENSE("GPL");
