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
#include <linux/slab.h> //定义kmalloc接口
#include <asm/io.h>//定义virt_to_phys接口
#include <linux/mm.h>//remap_pfn_range
#include <linux/fs.h>


#define MMAP_TEST_MODULE_NAME "mmap_test"
#define MMAP_TEST_CLASS_NAME "mmap_test_class"
#define MMAP_TEST_CLASS_DEVICE_NAME "mmap_test"
#define MMAP_TEST_CHRDEV_REGION 1
#define MMAP_TEST_SIZE 4096

static int MMAP_TEST_MAJOR = 0;
static struct class *mmap_test_class = NULL;
static struct device *mmap_test_class_device = NULL;
static struct cdev mmap_test_cdev;
static char *buf = NULL;


static int mmap_test_open(struct inode *inode, struct file *file)
{
	printk("device open\n");
	buf = (char *)kmalloc(MMAP_TEST_SIZE, GFP_KERNEL);//内核申请内存只能按页申请，申请该内存以便后面把它当作虚拟设备
	return 0;
}

static int mmap_test_release(struct inode *indoe, struct file *file)
{
	printk("device close\n");
	if(buf)
	{
		kfree(buf);
	}
	return 0;
}

static int mmap_test_mmap(struct file *file, struct vm_area_struct *vma)
{
	vma->vm_flags |= VM_IO;//表示对设备IO空间的映射
	//vma->vm_flags |= VM_RESERVED;//标志该内存区不能被换出，在设备驱动中虚拟页和物理页的关系应该是长期的，应该保留起来，不能随便被别的虚拟页换出
	if(remap_pfn_range(vma,//虚拟内存区域，即设备地址将要映射到这里
					   vma->vm_start,//虚拟空间的起始地址
					   virt_to_phys(buf)>>PAGE_SHIFT,//与物理内存对应的页帧号，物理地址右移12位
					   vma->vm_end - vma->vm_start,//映射区域大小，一般是页大小的整数倍
					   vma->vm_page_prot))//保护属性，
	{
		return -EAGAIN;
	}
	return 0;
}


static struct file_operations mmap_test_fops = {
    .owner = THIS_MODULE,
    .open = mmap_test_open,
    .release = mmap_test_release,
    .mmap = mmap_test_mmap,
};

static int mmap_test_init(void)
{
    int ret = 0;
    dev_t dev;

    if (MMAP_TEST_MAJOR)
    {
        dev = MKDEV(MMAP_TEST_MAJOR, 0);
        ret = register_chrdev_region(dev, MMAP_TEST_CHRDEV_REGION, MMAP_TEST_MODULE_NAME);
        if (ret)
        {
            printk("reg chrdev region fail \n");
            return -1;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&dev, 0, MMAP_TEST_CHRDEV_REGION, MMAP_TEST_MODULE_NAME);
        if (ret)
        {
            printk("alloc chrdev region fail \n");
            return -1;
        }
        MMAP_TEST_MAJOR = MAJOR(dev);
    }

    cdev_init(&mmap_test_cdev, &mmap_test_fops);
    mmap_test_cdev.owner = THIS_MODULE;

    ret = cdev_add(&mmap_test_cdev, dev, MMAP_TEST_CHRDEV_REGION);
    if (ret)
    {
        printk("cdev add fail \n");
        return -1;
    }

    mmap_test_class = class_create(THIS_MODULE, MMAP_TEST_CLASS_NAME);
    if (!mmap_test_class)
    {
        printk("class create fail \n");
        goto err_free_chrdev;
    }

    mmap_test_class_device = device_create(mmap_test_class, NULL, dev, NULL, MMAP_TEST_CLASS_DEVICE_NAME);
    if (!mmap_test_class_device)
    {
        printk("class device create fail \n");
        goto err_free_class;
    }

    printk("%s init ok \n", MMAP_TEST_MODULE_NAME);

    return ret;

err_free_class:
    class_destroy(mmap_test_class);

err_free_chrdev:
    cdev_del(&mmap_test_cdev);

    unregister_chrdev_region(dev, MMAP_TEST_CHRDEV_REGION);

    return -1;
}

static void mmap_test_exit(void)
{
    dev_t dev;
    dev = MKDEV(MMAP_TEST_MAJOR, 0);

    cdev_del(&mmap_test_cdev);

    unregister_chrdev_region(dev, MMAP_TEST_CHRDEV_REGION);

    device_destroy(mmap_test_class, dev);

    class_destroy(mmap_test_class);

    printk("%s exit \n", MMAP_TEST_MODULE_NAME);
}

module_init(mmap_test_init);
module_exit(mmap_test_exit);

MODULE_LICENSE("GPL");

