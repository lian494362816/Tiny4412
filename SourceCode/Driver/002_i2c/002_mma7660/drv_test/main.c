#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include "../my_s3c4412.h"
#include "../my_error.h"

#define INPUT_FILE "/dev/mma7660"
#define BUFF_SIZE 0x20
#define READ (1)
#define WRITE (2)


static int iic_write(int fd, unsigned char addr, unsigned char *buf, int count)
{
    int i = 0;
    int ret = 0;
    //tmp_buff[0] = addr
    //tmp_buff[1] = value
    unsigned char tmp_buff[2];

    if (!buf)
    {
        PRINT_ERR("buf is NULL \n");
        return 0;
    }

    if ((int) addr + count > 0xFF)
    {
        PRINT_WRN("addr + count too big:%d \n", addr + count);
        count = 0xFF - addr;
    }

    for (i = 0; i < count; i++)
    {
        tmp_buff[0] = addr + i;
        tmp_buff[1] = buf[i];
        //PRINT_INFO("addr:%x \n", tmp_buff[0]);
        //PRINT_INFO("value:%x \n", tmp_buff[1]);
        ret = write(fd, tmp_buff, sizeof(tmp_buff));
        if (ret < 0)
        {
            PRINT_ERR("write fail \n");
            return ret;
        }
    }

    return 0;
}

static int iic_read(int fd, unsigned char addr, unsigned char *buf, int count)
{

    int i = 0;
    int ret = 0;
    //tmp_buff[0] = addr
    //tmp_buff[1] = value
    unsigned char tmp_buff[2];

    if (!buf)
    {
        PRINT_ERR("buf is NULL \n");
        return 0;
    }

    if ((int) addr + count > 0xFF)
    {
        PRINT_WRN("addr + count too big:%d \n", addr + count);
        count = 0xFF - addr;
    }

    for (i = 0; i < count; i++)
    {
        tmp_buff[0] = addr + i;
        ret = read(fd, tmp_buff, sizeof(tmp_buff));
        if (ret < 0)
        {
            PRINT_ERR("write fail \n");
            return ret;
        }
        memcpy(buf + i, &tmp_buff[1], sizeof(char));
        //PRINT_INFO("tmp_buff[i]:%x \n", tmp_buff[1]);
    }

    return 0;
}

static void help(char *str)
{           //  0   1   2      3
    printf("Use %s <w> <addr> <value> \n", str);
    printf("Use %s <r> <addr> [count] \n", str);
}

int main(int argc, char *argv[])
{
    int fd = 0;
    int tmp_buff[3] = {0};
    int ret = 0;

    fd = open(INPUT_FILE, O_RDWR);
    if (fd <= 0)
    {
        PRINT_ERR("open %s fail \n", INPUT_FILE);
        return fd;
    }

    while(1)
    {
        ret = read(fd, tmp_buff, sizeof(tmp_buff));
        if (ret < 0)
        {
            PRINT_ERR("write fail \n");
            return ret;
        }

        printf("x:%3d,y:%3d,z:%3d \n", tmp_buff[0], tmp_buff[1], tmp_buff[2]);

        usleep(1000 * 200);
    }

    close(fd);

    return 0;
}
