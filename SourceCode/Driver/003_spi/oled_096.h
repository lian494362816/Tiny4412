#ifndef OLED_096_H__
#define OLED_096_H__


#include <linux/cdev.h>
#include <linux/spi/spi.h>
#include "my_error.h"
#include "my_s3c4412.h"

struct oled_t {
    struct cdev cdev;
    struct device *device;
    atomic_t atomic;
    unsigned int major;
    struct s3c4412_gpio *gpio;
    struct spi_device *spi;
};


extern struct oled_t *oled_alloc(void);
extern void oled_free(struct oled_t *oled);
extern int oled_write_data(struct oled_t *oled, void *const buf, size_t len);
extern int oled_write_cmd(struct oled_t *oled, unsigned char cmd);
extern int oled_hw_init(struct oled_t *oled);
extern int oled_hw_deinit(struct oled_t *oled);
extern int oled_clear(struct oled_t *oled);
extern int oled_write_byte(struct oled_t *oled, unsigned char data);
extern int oled_set_pos(struct oled_t *oled, int x, int y);
extern void oled_show_char(struct oled_t *oled, int x, int y, int chr);
extern void oled_show_string(struct oled_t *oled, int x, int y, char *chr);
extern void oled_show_chinese(struct oled_t *oled, int x, int y, int no);


#endif /* OLED_096_H__ */
