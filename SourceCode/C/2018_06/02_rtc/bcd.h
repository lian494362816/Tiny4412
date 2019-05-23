#ifndef __BCD_H__
#define __BCD_H__

#include "datatype.h"

static inline U32 bcd2bin(U8 u8Value)
{
	return ( ((u8Value) & 0xF) + (((u8Value) >> 4) * 10) );
}

static inline U8 bin2bcd(U32 u32Value)
{
	return ( ((u32Value / 10) << 4) | (u32Value % 10) );
}

#endif //__BCD_H__
