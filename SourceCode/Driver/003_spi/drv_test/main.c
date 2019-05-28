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

#include "../iconv.h"
#include "../my_s3c4412.h"
#include "../my_error.h"
#include "../oled_ioctl.h"
#include "../oled_096_front.h"

//测试oled

#define INPUT_FILE "/dev/oled096_0"
#define HZK16 "./HZK16"

/*
need include <iconv.h> <string.h>
convert UTF-8 to GBK (GBK is compatible GB2313)
*/
int UTF8_to_GBK(unsigned char *input_buf, size_t input_len, unsigned char *output_buf, size_t output_len)
{
    char *encTo = "GBK//IGNORE";
    char *encFrom = "UTF-8";

    char *srcstart = input_buf;
    char *tempoutbuf = output_buf;
    int i = 0;
    long ret = 0;

    iconv_t fd = iconv_open (encTo, encFrom);
    if (fd == (iconv_t)-1)
    {
      perror ("iconv_open fail \n");
      return -1;
    }

    memset(output_buf, 0, output_len);

    ret = iconv (fd, &srcstart, &input_len, &tempoutbuf, &output_len);
    if (ret == -1)
    {
      perror ("iconv fail \n");
    }

    iconv_close (fd);

    return 0;
}

/*
buf: HZK16 buf,
len: buf len,should be multiple of 32
*/
int print_HZK16_string(unsigned char *buf, int len)
{
    int i, j, k, n, z;
    int flag;
    unsigned char key[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

    if (len < 32 || 0 != (len % 32))
    {
        printf("len is not multiple of 32 \n");
        return -1;
    }

    n = 0;
    do{
        for(k=0; k < 16; k++)
        {
            for(j=0; j < 2; j++)
            {
                for(i=0; i < 8; i++)
                {
                    flag = buf[k*2+j]&key[i];
                    printf("%s", flag?"●":"○");
                }
            }
            printf("\n");
        }
        for (i = 0; i <32; i++)
        {
            printf("0x%-2x ", buf[i]);
        }
        n++;
        buf += 32;
        printf(" \n");
        printf(" \n");
    }while(n < len / 32);

    return 0;
}

/*
one_chinese_buf:input, only one chinese-character(UTF-8), 3 bytes
hex_buf:output, HZK16 code, 32 bytes
can use print_HZK16_string to print hex_buf result
*/
int Get_GBK_Code(unsigned char *one_chinese_buf, unsigned char *hex_buf)
{
    FILE* fphzk = NULL;
    unsigned char tmp_buf[10];
    int offset;

    //one chinese-character(UTF-8) is 3 bytes
    UTF8_to_GBK(one_chinese_buf, 3, tmp_buf, 10);

    fphzk = fopen("HZK16", "rb");
    if(fphzk == NULL)
    {
        fprintf(stderr, "error hzk16\n");
        return 1;
    }

    offset = (94*(unsigned int)(tmp_buf[0]-0xa0-1)+(tmp_buf[1]-0xa0-1))*32;

    fseek(fphzk, offset, SEEK_SET);
    fread(hex_buf, 1, 32, fphzk);

    //print_HZK16_string(hex_buf, 32);

    fclose(fphzk);

    return 0;
}

int Get_GBK_String(unsigned char *chinese_buf, int chinese_buf_len, unsigned char *hex_buf, int hex_buf_len)
{
    int i = 0;
    /*
    one UTF-8 character is 3 bytes
    one HZK16 character need 16*16 bit, 16*16 bit = 32bytes
    */

    if (chinese_buf_len % 3)
    {
        printf("chinese_buf_len is not multiple of 3 \n");
    }

    if (hex_buf_len < chinese_buf_len / 3 * 32)
    {
        printf("hex_buf_len is not large, need:%d \n",  chinese_buf_len / 3 * 32);
        return -1;
    }

    //only need multiple of 32
    hex_buf_len = hex_buf_len - (hex_buf_len % 32);
    //printf("chinese_buf_len:%d \n", chinese_buf_len);
    for (i = 0; i < chinese_buf_len;)
    {
        Get_GBK_Code(chinese_buf + i, hex_buf + (i / 3 * 32));
        i += 3;
    }

    return 0;
}

int change_arrangement_mode(unsigned char *pc_buf, unsigned char *oled_buf, int len)
{
    int i, j, k;

    if (len % 32)
    {
        printf("len is not multiple of 32 \n");
        return -1;
    }

    k = 0;
    do {
        for (i = 0; i < 8; i++)
        {
            for (j = 0; j < 8; j++)
            {
                oled_buf[i] |= ( ((pc_buf[j * 2] & (0x1 << 7 - (i % 8))) >> (7 - (i % 8)) ) << j );
                oled_buf[i + 8] |= ( ((pc_buf[j * 2 + 1] & (0x1 << 7 - (i % 8))) >> (7 - (i % 8)) ) << j );
                oled_buf[i + 16] |= ( ((pc_buf[j * 2 + 16] & (0x1 << 7 - (i % 8))) >> (7 - (i % 8)) ) << j );
                oled_buf[i + 24] |= ( ((pc_buf[j * 2 + 1 + 16] & (0x1 << 7 - (i % 8))) >> (7 - (i % 8)) ) << j );
                //printf("buf[%d] bit[%d] = byte[%d] & %0x << %d \n", i, j, j * 2, (0x1 << 7 - (i % 8)), j);
            }
         }
         k += 32;
         pc_buf += 32;
         oled_buf += 32;
     }while(k < len);

    pc_buf -= k;
    oled_buf -= k;

     return 0;
}

int write_data(int fd, char *buf, int len)
{
    return write(fd, buf, len);
}

int write_byte(int fd, unsigned char data)
{
    return write(fd, &data, 1);
}

int set_pos(int fd, int x, int y)
{
    struct oled_pos_t pos = {x, y};

    return ioctl(fd, OLED_SET_POS, &pos);
}

int write_cmd(int fd, unsigned char cmd)
{
    return ioctl(fd, OLED_WRITE_CMD, &cmd);
}

int oled_off(int fd)
{
    return ioctl(fd, OLED_OFF);
}

int oled_on(int fd)
{
    return ioctl(fd, OLED_ON);
}

void show_char(int fd, int x, int y, int chr)
{
    unsigned char c=0,i=0;
    c=chr-' ';//得到偏移后的值

    if (x > 128-1) {x=0;y=y+2;}
    {
        set_pos(fd, x, y);
        for(i=0;i<8;i++)
        {
            write_byte(fd, F8X16[c*16+i]);
        }

        y = y + 1;
         set_pos(fd, x, y);
        for(i=0;i<8;i++)
        {
            write_byte(fd, F8X16[c*16+i+8]);
        }
    }
}

void oled_show_chinese(int fd, int x, int y, unsigned char *buf, int len)
{
    int t;
    set_pos(fd, x,y);
    for(t=0;t<16;t++)
    {
        write_byte(fd, buf[t]);
    }

    set_pos(fd, x,y + 1);
    for(t=0;t<16;t++)
    {
        write_byte(fd, buf[16 + t]);
    }
}

#define BUF_LEN (32 * 3)

int main(int argc, char *argv[])
{
    int fd = 0;
    int i = 0;
    unsigned char utf8[10] = "嵌入式";
    unsigned char gbk_code[BUF_LEN] = {0};
    unsigned char oled_buf[BUF_LEN] = {0};

    fd = open(INPUT_FILE, O_RDWR);
    if (fd <= 0)
    {
        PRINT_FUNC_ERR(open);
        return fd;
    }

    //Get_GBK_Code(utf8, gbk_code);
    Get_GBK_String(utf8, strlen(utf8), gbk_code, BUF_LEN);

    change_arrangement_mode(gbk_code, oled_buf, BUF_LEN);

    //show_char(fd, 0 ,0, 'A');
    show_char(fd, 0 ,2, 'B');
    show_char(fd, 0 ,4, 'C');
    show_char(fd, 0 ,6, 'D');

    oled_show_chinese(fd, 0, 0, oled_buf, 32);
    oled_show_chinese(fd, 16, 0, oled_buf + 32, 32);
    oled_show_chinese(fd, 32, 0, oled_buf + 64, 32);
    while(1)
    {
        oled_off(fd);
        usleep(1000 * 500);
        oled_on(fd);
        usleep(1000 * 500);
    }

    return 0;
}




