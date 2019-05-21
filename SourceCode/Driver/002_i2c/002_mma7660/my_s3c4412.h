#ifndef __MY_S3C4412_H
#define __MY_S3C4412_H

typedef struct s3c4412_gpio{
    long con;    //  configuration register
    long dat;    //  data register
    long pud;    //  pull-up/ pull-down register
    long drv;    //  drive strength control register
    long conpnd; //  power down mode configuration register
    long pudpnd; //  power down mode pull-up/ pull-down register
}s3c4412_gpio_t;


#endif /* __MY_S3C4412_H */
