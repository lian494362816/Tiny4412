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

#define INPUT_FILE "/dev/led1"


int main(int argc, char *argv[])
{
    int fd = 0;
    ret = 0;

    fd = open(INPUT_FILE, O_RDWR);
    if (fd < 0)
    {
        PRINT_ERR("open fail:%s \n", INPUT_FILE);
    }

    ret = ioctl(fd, S3C4412_LED_ON);

    close(fd);

    return 0;
}


