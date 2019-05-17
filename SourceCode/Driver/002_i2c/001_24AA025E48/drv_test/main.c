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

#define INPUT_FILE "/dev/24aa025e48"
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
    int i = 0;
    int ret = 0;
    int opt = 0;
    int count = 1;
    unsigned char addr = 0;
    unsigned char value = 0;

    if (argc < 2)
    {
        help(argv[0]);
        return -1;
    }

    if (!strcmp("w", argv[1]))
    {
        //printf("write opt \n");
        opt = WRITE;
    }
    else if (!strcmp("r", argv[1]))
    {
        //printf("read opt \n");
        opt = READ;
    }
    else
    {
        PRINT_WRN("not find 'w' or 'r' \n");
        help(argv[0]);
        return -1;
    }

    fd = open(INPUT_FILE, O_RDWR);
    if (fd <= 0)
    {
        PRINT_ERR("open %s fail \n", INPUT_FILE);
        return fd;
    }

    addr = (unsigned char)strtoul(argv[2], NULL, 16);
    if (WRITE == opt)
    {
        value = (unsigned char)strtoul(argv[3], NULL, 16);
        ret = iic_write(fd, addr, &value, 1);
        if (ret < 0)
        {
            PRINT_ERR("iic write fail \n");
            close(fd);
            return -1;
        }
        PRINT_INFO("addr:0x%-2x, value:0x%-2x \n", addr, value);
    }

    if (READ == opt)
    {
        if (argc > 3)
        {
            count = (unsigned char)strtoul(argv[3], NULL, 10);
        }

        for (i = 0; i < count ; i++)
        {
            ret = iic_read(fd, addr + i, &value, 1);
            if (ret < 0)
            {
                PRINT_ERR("iic read fail \n");
                close(fd);
                return -1;
            }

            if (!(i % 8))
            {
                printf("\n");
                printf("[0x%-2x] ", addr + i);
            }
            printf("0x%-2x  ", value);
        }
        printf("\n");
    }

#if 0
    unsigned char buff[BUFF_SIZE] = {0};
    for (i = 0; i < BUFF_SIZE; i++)
    {
        memset(buff, 0x55, BUFF_SIZE);
    }

    for (i = 0; i < BUFF_SIZE; i++)
    {
        ret = iic_write(fd, i, buff, BUFF_SIZE);
        if (ret < 0)
        {
            PRINT_ERR("iic_write fail \n");
            close(fd);
            return -1;
        }
    }

    for (i = 0; i < BUFF_SIZE; i++)
    {
        memset(buff, 0x0, BUFF_SIZE);
    }

    for (i = 0; i < BUFF_SIZE; i++)
    {
        ret = iic_read(fd, i, buff, BUFF_SIZE);
        if (ret < 0)
        {
            PRINT_ERR("iic_write fail \n");
            close(fd);
            return -1;
        }

        if (0x55 != buff[i])
        {
            PRINT_INFO("buff[%d]:%d \n", i, buff[i]);
            break;
        }
    }
#endif

    close(fd);

    return 0;
}
