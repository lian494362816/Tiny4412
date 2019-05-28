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
#include <linux/spi/spi.h>
#include <linux/delay.h>
#include "oled_096.h"
#include "my_s3c4412.h"
#include "oled_096_front.h"

/*
    OLED          Tiny4412
    D/C           GPB_5
    RESET         GPB_4
*/

#define GPIO_B_BASE (0x11400000 + 0x40)
#define GPIO_DATA_CMD_PIN (5)
#define GPIO_RESET_PIN (4)


#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF
#define X_WIDTH 	128
#define Y_WIDTH 	64


struct oled_t *oled_alloc(void)
{
    struct oled_t *oled = NULL;
    oled = kmalloc(sizeof(struct oled_t), GFP_KERNEL);
    if (!oled)
    {
        PRINT_FUNC_ERR(kmalloc);
        return NULL;
    }

    oled->tmp_buff = kmalloc(TMP_BUFF_SIZE, GFP_KERNEL);
    if (!oled->tmp_buff)
    {
        PRINT_FUNC_ERR(kmalloc);
        goto err_free_oled;
    }

    oled->gpio = ioremap(GPIO_B_BASE, sizeof(struct s3c4412_gpio));
    if (NULL == oled->gpio)
    {
        PRINT_FUNC_ERR(ioremap);
        goto err_free_buff;
    }

    return oled;
err_free_buff:
    kfree(oled->tmp_buff);

err_free_oled:
    kfree(oled);

    return NULL;
}
EXPORT_SYMBOL_GPL(oled_alloc);

void oled_free(struct oled_t *oled)
{
    if (!oled)
    {
        iounmap(oled->gpio);
        kfree(oled->tmp_buff);
        kfree(oled);
    }
}
EXPORT_SYMBOL_GPL(oled_free);


int oled_write_data(struct oled_t *oled, void *const buf, size_t len)
{
    int ret = -1;
    oled->gpio->dat |= (0x1 << GPIO_DATA_CMD_PIN);

    ret = spi_write(oled->spi, buf, len);
    if (ret)
    {
        PRINT_FUNC_ERR(spi_write);
    }

    return ret;
}
EXPORT_SYMBOL_GPL(oled_write_data);

int oled_write_byte(struct oled_t *oled, unsigned char data)
{
    int ret = -1;
    oled->gpio->dat |= (0x1 << GPIO_DATA_CMD_PIN);

    ret = spi_write(oled->spi, &data, 1);
    if (ret)
    {
        PRINT_FUNC_ERR(spi_write);
    }

    return ret;
}
EXPORT_SYMBOL_GPL(oled_write_byte);


int oled_write_cmd(struct oled_t *oled, unsigned char cmd)
{
    int ret = -1;

    oled->gpio->dat &= ~(0x1 << GPIO_DATA_CMD_PIN);

    ret = spi_write(oled->spi, &cmd, 1);
    if (ret)
    {
        PRINT_FUNC_ERR(spi_write);
    }

    return ret;
}
EXPORT_SYMBOL_GPL(oled_write_cmd);

int oled_clear(struct oled_t *oled)
{
    int i = 0;
    unsigned char buf[128] = {0};
    for(i=0; i<8; i++)
    {
        oled_write_cmd(oled, 0xb0+i); //page addr
        oled_write_cmd(oled, 0x00); // column low start addr
        oled_write_cmd(oled ,0x10); // column high start addr
        oled_write_data(oled, buf, 128);
    }

    return 0;
}
EXPORT_SYMBOL_GPL(oled_clear);


int oled_hw_init(struct oled_t *oled)
{
    /* set D/C RESET pin to output mode
    D/C           GPB_5
    RESET         GPB_4
    */

    oled->gpio->con &= ~(0xF << (GPIO_DATA_CMD_PIN * 4));
    oled->gpio->con |= (0x1 << (GPIO_DATA_CMD_PIN * 4));

    oled->gpio->con &= ~(0xF << (GPIO_RESET_PIN * 4));
    oled->gpio->con |= (0x1 << (GPIO_RESET_PIN * 4));

    /* reset oled */
    oled->gpio->dat &= ~(0x1 << GPIO_RESET_PIN);
    msleep(200);
    oled->gpio->dat |= (0x1 << GPIO_RESET_PIN);

    oled_write_cmd(oled, 0xAE);//--turn off oled panel
    oled_write_cmd(oled, 0x00);//---set low column address
    oled_write_cmd(oled, 0x10);//---set high column address
    oled_write_cmd(oled, 0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    oled_write_cmd(oled, 0x81);//--set contrast control register
    oled_write_cmd(oled, 0xCF); // Set SEG Output Current Brightness
    oled_write_cmd(oled, 0xA1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    oled_write_cmd(oled, 0xC8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    oled_write_cmd(oled, 0xA6);//--set normal display
    oled_write_cmd(oled, 0xA8);//--set multiplex ratio(1 to 64)
    oled_write_cmd(oled, 0x3f);//--1/64 duty
    oled_write_cmd(oled, 0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    oled_write_cmd(oled, 0x00);//-not offset
    oled_write_cmd(oled, 0xd5);//--set display clock divide ratio/oscillator frequency
    oled_write_cmd(oled, 0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    oled_write_cmd(oled, 0xD9);//--set pre-charge period
    oled_write_cmd(oled, 0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    oled_write_cmd(oled, 0xDA);//--set com pins hardware configuration
    oled_write_cmd(oled, 0x12);
    oled_write_cmd(oled, 0xDB);//--set vcomh
    oled_write_cmd(oled, 0x40);//Set VCOM Deselect Level
    oled_write_cmd(oled, 0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    oled_write_cmd(oled, 0x02);//
    oled_write_cmd(oled, 0x8D);//--set Charge Pump enable/disable
    oled_write_cmd(oled, 0x14);//--set(0x10) disable
    oled_write_cmd(oled, 0xA4);// Disable Entire Display On (0xa4/0xa5)
    oled_write_cmd(oled, 0xA6);// Disable Inverse Display On (0xa6/a7)
    oled_write_cmd(oled, 0xAF);//--turn on oled panel

    oled_write_cmd(oled, 0xAF); /*display ON*/

    oled_clear(oled);

    return 0;
}
EXPORT_SYMBOL_GPL(oled_hw_init);


int oled_hw_deinit(struct oled_t *oled)
{
    oled_write_cmd(oled, 0xAE);//--turn off oled panel

    /* reset oled */
    oled->gpio->dat &= ~(0x1 << GPIO_RESET_PIN);
    msleep(200);
    oled->gpio->dat |= (0x1 << GPIO_RESET_PIN);

    oled_clear(oled);

    return 0;
}
EXPORT_SYMBOL_GPL(oled_hw_deinit);

int oled_set_pos(struct oled_t *oled, int x, int y)
{
    oled_write_cmd(oled, 0xb0+y);
    oled_write_cmd(oled, ((x&0xf0)>>4)|0x10);
    oled_write_cmd(oled, (x&0x0f)|0x01);

    return 0;
}
EXPORT_SYMBOL_GPL(oled_set_pos);


//开启OLED显示
void oled_display_on(struct oled_t *oled)
{
    oled_write_cmd(oled, 0X8D);  //SET DCDC命令
    oled_write_cmd(oled, 0X14);  //DCDC ON
    oled_write_cmd(oled, 0XAF);  //DISPLAY ON
}
EXPORT_SYMBOL_GPL(oled_display_on);

//关闭OLED显示
void oled_display_off(struct oled_t *oled)
{
    oled_write_cmd(oled, 0X8D);  //SET DCDC命令
    oled_write_cmd(oled, 0X10);  //DCDC OFF
    oled_write_cmd(oled, 0XAE);  //DISPLAY OFF
}
EXPORT_SYMBOL_GPL(oled_display_off);

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63

void oled_show_char(struct oled_t *oled, int x, int y, int chr)
{
    unsigned char c=0,i=0;
    c=chr-' ';//得到偏移后的值

    if (x > Max_Column-1) {x=0;y=y+2;}
    {
        oled_set_pos(oled, x,y);
        for(i=0;i<8;i++)

        oled_write_byte(oled, F8X16[c*16+i]);
        oled_set_pos(oled, x,y+1);
        for(i=0;i<8;i++)
        {
            oled_write_byte(oled, F8X16[c*16+i+8]);
        }
    }
}
EXPORT_SYMBOL_GPL(oled_show_char);

void oled_show_string(struct oled_t *oled, int x, int y, char *chr)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {
        oled_show_char(oled, x,y,chr[j]);
        x+=8;
        if(x>120){x=0;y+=2;}
        j++;
    }
}
EXPORT_SYMBOL_GPL(oled_show_string);

//显示汉字
void oled_show_chinese(struct oled_t *oled, int x, int y, int no)
{
    u8 t,adder=0;
    oled_set_pos(oled, x,y);
    for(t=0;t<16;t++)
    {
        //oled_write_byte(oled, chinese_string[2*no][t]);
        adder+=1;
    }

    oled_set_pos(oled, x,y + 1);
    for(t=0;t<16;t++)
    {
        //oled_write_byte(oled,chinese_string[2*no+1][t]);
        adder+=1;
    }
}
EXPORT_SYMBOL_GPL(oled_show_chinese);


