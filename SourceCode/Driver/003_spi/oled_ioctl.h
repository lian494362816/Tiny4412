#ifndef __OLED_IOCTL_H
#define __OLED_IOCTL_H

struct oled_pos_t{
    int x;
    int y;
};

#define S3C4412_OLED_MAGIC 'o'

#define OLED_ON  _IO(S3C4412_OLED_MAGIC, 1)
#define OLED_OFF _IO(S3C4412_OLED_MAGIC, 2)
#define OLED_SET_POS _IOW(S3C4412_OLED_MAGIC, 3, struct oled_pos_t)
#define OLED_WRITE_CMD _IOW(S3C4412_OLED_MAGIC, 4, unsigned char)

#endif /* OLED_IOCTL_H */
