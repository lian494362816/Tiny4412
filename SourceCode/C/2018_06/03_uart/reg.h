#ifndef __REG_H__
#define __REG_H__

#include "datatype.h"

#define GPX3CON   *(VU32 *) (0x11000C60)  //key
#define GPX3DAT   *(VU32 *) (0x11000C64)

#define GPM4CON   *(VU32 *) (0x110002E0) //led
#define GPM4DAT   *(VU32 *) (0x110002E4) 

#define GPD0CON   *(VU32 *) (0x114000A0) //buzzer
#define GPD0DAT   *(VU32 *) (0x114000A4) 

#define  PRINT_ADDR   (void*) 0xc3e11ad0

//rtc
#define RTC_BASE_ADDRESS (0x10070000)
#define INTP         *(VU32 *) (RTC_BASE_ADDRESS + 0x0030)  
#define RTCCON       *(VU32 *) (RTC_BASE_ADDRESS + 0x0040)  
#define TICCNT       *(VU32 *) (RTC_BASE_ADDRESS + 0x0044)  
#define RTCALM       *(VU32 *) (RTC_BASE_ADDRESS + 0x0050)  
#define ALMSEC       *(VU32 *) (RTC_BASE_ADDRESS + 0x0054)  
#define ALMMIN       *(VU32 *) (RTC_BASE_ADDRESS + 0x0058)  
#define ALMHOUR      *(VU32 *) (RTC_BASE_ADDRESS + 0x005C)  
#define ALMDAY       *(VU32 *) (RTC_BASE_ADDRESS + 0x0060)  
#define ALMMON       *(VU32 *) (RTC_BASE_ADDRESS + 0x0064)  
#define ALMYEAR      *(VU32 *) (RTC_BASE_ADDRESS + 0x0068)  
#define BCDSEC       *(VU32 *) (RTC_BASE_ADDRESS + 0x0070)  
#define BCDMIN       *(VU32 *) (RTC_BASE_ADDRESS + 0x0074)  
#define BCDHOUR      *(VU32 *) (RTC_BASE_ADDRESS + 0x0078)  
#define BCDDAY       *(VU32 *) (RTC_BASE_ADDRESS + 0x007C)  
#define BCDDAYWEEK   *(VU32 *) (RTC_BASE_ADDRESS + 0x0080)  
#define BCDMON       *(VU32 *) (RTC_BASE_ADDRESS + 0x0084)  
#define BCDYEAR      *(VU32 *) (RTC_BASE_ADDRESS + 0x0088)  
#define CURTICCNT    *(VU32 *) (RTC_BASE_ADDRESS + 0x0090)  
 
#endif
