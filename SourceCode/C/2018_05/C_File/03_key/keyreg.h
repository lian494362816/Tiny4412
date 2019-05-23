#ifndef __KEYREG_H__
#define __KEYREG_H__

typedef volatile unsigned int     U32;
typedef volatile unsigned short   U16;
typedef volatile unsigned char    U8;

#define GPX3CON       *(U32 *)  (0x11000C60)
#define GPX3DAT       *(U32 *)  (0x11000C64)

#endif
