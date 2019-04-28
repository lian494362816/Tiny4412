#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "../my_s3c4412.h"
#include "../my_error.h"
#include <stdlib.h>

//循环点亮led

#define INPUT_FILE "/dev/led%d"

#define MAX_LED (10)

int main(int argc, char *argv[])
{
    int fd[MAX_LED] = {0};
    int ret = 0;
    unsigned char file_name[128] = {0};
    int i = 0;

    for (i = 0; i < MAX_LED; i++)
    {
        memset(file_name, 0, 128);
        sprintf(file_name, INPUT_FILE, i);
        //printf("file name:%s \n", file_name);
        fd[i] = open(file_name, O_RDWR);
        if (fd[i] > 0)
        {
            PRINT_INFO("open %s \n", file_name);
        }
    }

    //is any file open ok?
    for (i = 0; i < MAX_LED; i++)
    {
        if (fd[i] > 0)
        {
            break;
        }
    }

    if (MAX_LED == i)
    {
        PRINT_ERR("not file open");
        return -1;
    }

    for (i = 0; i < MAX_LED; i++)
    {
        if (fd[i] >0)
        {
            ret = ioctl(fd[i], S3C4412_LED_ON);
            usleep(1000 * 500);
            ret = ioctl(fd[i], S3C4412_LED_OFF);
            usleep(1000 * 500);
        }
    }

    for (i = 0; i < MAX_LED; i++)
    {
        if (fd[i] > 0)
        {
            close(fd[i]);
        }

        fd[i] = 0;
    }

    return 0;
}



