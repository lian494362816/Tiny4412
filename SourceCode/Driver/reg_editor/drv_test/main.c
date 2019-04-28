#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "reg_editor.h"
#include "my_error.h"
#include <stdlib.h>

#define INPUT_FILE "/dev/reg_editor"
#define R8_STR  "r8"
#define R16_STR "r16"
#define R32_STR "r32"

#define W8_STR  "w8"
#define W16_STR "w16"
#define W32_STR "w32"


#define FILE_FILE 0
#define READ_WRITE 1
#define ADDRESS 2
#define NUM_VAL 3


struct option_t{
    char *str;
    unsigned int cmd;
};

static void help(char *str)
{
    printf("Use:%s <r8|r16|r32> <reg_address> [num] \n", str);
    printf("Use:%s <w8|w16|w32> <reg_address> <val>  \n", str);
}

static struct option_t options[] = {
    {R8_STR,  REG_EDITOR_R8},
    {R16_STR, REG_EDITOR_R16},
    {R32_STR, REG_EDITOR_R32},

    {W8_STR,  REG_EDITOR_W8},
    {W16_STR, REG_EDITOR_W16},
    {W32_STR, REG_EDITOR_W32},
};

int main(int argc, char *argv[])
{
    int fd = 0;
    int i = 0;
    int ret = 0;
    struct reg_editor_t reg_editor;
    unsigned int cmd = 0;
    int num = 1;

    if (argc <= 2)
    {
        help(argv[FILE_FILE]);
        return -1;
    }

    //argv[ADDRESS] reg_address
    reg_editor.addr = strtoul(argv[ADDRESS], NULL, 16);

    //argv[READ_WRITE] option
    for (i = 0; i < sizeof(options) / sizeof(options[0]); i++)
    {
        if (!strncmp(argv[READ_WRITE], options[i].str, strlen(options[i].str)))
        {
            cmd = options[i].cmd;
            //printf("str:%s \n", options[i].str);
        }
    }

    if (0 == cmd)
    {
        printf("no cmd compare \n");
        help(argv[0]);
        return -1;
    }

    //write
    if (strstr(argv[READ_WRITE], "w"))
    {
        if (argc < 4)
        {
            printf("no write value\n");
            help(argv[0]);
            return -1;
        }
        //argv[NUM_VAL] val/num
        reg_editor.val = strtoul(argv[NUM_VAL], NULL, 16);
    }
    else
    {
        if (argc > 3)
        {
            //argv[NUM_VAL] val/num
            num = strtoul(argv[NUM_VAL], NULL, 10);
        }
    }

    fd = open(INPUT_FILE, O_RDWR);
    if (fd <= 0)
    {
        printf("open fail:%s \n", INPUT_FILE);
        return -1;
    }

    if (num <= 0)
    {
        num = 1;
    }

    for (i = 0; i < num; i++)
    {
        ret = ioctl(fd, cmd, &reg_editor);
        if (ret)
        {
            printf("ioctl fail \n");
            close(fd);
            return -1;
        }

        printf("[0x%x]:0x%x \n", reg_editor.addr, reg_editor.val);

        if (REG_EDITOR_R8 == cmd)
        {
            reg_editor.addr += 1;
        }

#if 0
        //for read
        if (cmd == REG_EDITOR_R8)
        {
            //reg_editor.addr += 1;
        }
        else if (cmd == REG_EDITOR_R16)
        {
            //reg_editor.addr += 2;
        }
        else if (cmd == REG_EDITOR_R32)
        {
            //reg_editor.addr += 4;
        }
        else
        {
            break;
        }
#endif
    }

    close(fd);

    return 0;
}


