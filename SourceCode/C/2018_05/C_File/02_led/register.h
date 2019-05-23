#ifndef __REGISTER_H__
#define __REGISTER_H__

typedef volatile unsigned int    U32;
typedef volatile unsigned short  U16;
typedef volatile unsigned char   U8;

#define GPM4CON  *(U32 *)(0x110002E0)
#define GPM4DAT  *(U32 *)(0x110002E4)

#endif
