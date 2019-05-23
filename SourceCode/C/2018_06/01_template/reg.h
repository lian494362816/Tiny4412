#ifndef __REG_H__
#define __REG_H__

typedef volatile unsigned int    U32;
typedef volatile unsigned short  U16;
typedef volatile unsigned char   U8;

typedef unsigned long ulong;

#define GPX3CON   *(U32 *) (0x11000C60)  //key
#define GPX3DAT   *(U32 *) (0x11000C64)

#define GPM4CON   *(U32 *) (0x110002E0) //led
#define GPM4DAT   *(U32 *) (0x110002E4) 

#define GPD0CON   *(U32 *) (0x114000A0) //buzzer
#define GPD0DAT   *(U32 *) (0x114000A4) 

#endif
