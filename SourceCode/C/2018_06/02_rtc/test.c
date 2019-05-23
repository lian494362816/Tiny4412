#include "basedevice.h"
#include "datatype.h"
#include "rtc.h"
#include "reg.h"

static PRINT_T printf = PRINT_ADDR;

static void rtc_print(rtc_time_t *pstRtcTime);

int _start(void)
{
	rtc_time_t stRtc;

	// U32 u32Sec;  [0, 59] 
	// U32 u32Min;  [0, 59] 
	// U32 u32Hour; [0, 23] 
	// U32 u32DayWeek; [1, 7] 
	// U32 u32Day;  [1, 31] 
	// U32 u32Mon;  [1, 12] 
	// U32 u32Year; [0, 99] ,the start year is 2000

	rtc_get_time(&stRtc);
	rtc_print(&stRtc);
	printf("===========\n");
	stRtc.u32Sec = 58;
	stRtc.u32Min = 59;
	stRtc.u32Hour = 23;
	stRtc.u32DayWeek = 7;
	stRtc.u32Day = 28;
	stRtc.u32Mon = 2;
	stRtc.u32Year = 2016;

	rtc_set_time(&stRtc);
	rtc_get_time(&stRtc);
	rtc_print(&stRtc);

#if 0
	rtc_reset_time();
	rtc_get_time(&stRtc);
	rtc_print(&stRtc);
#endif
	
	return 0;
}

static void rtc_print(rtc_time_t *pstRtcTime)
{
	printf("Sec:%d\n", pstRtcTime->u32Sec);
	printf("Min:%d\n", pstRtcTime->u32Min);
	printf("Hour:%d\n", pstRtcTime->u32Hour);
	printf("DayWeek:%d\n", pstRtcTime->u32DayWeek);
	printf("Day:%d\n", pstRtcTime->u32Day);
	printf("Mon:%d\n", pstRtcTime->u32Mon);
	printf("Year:%d\n", pstRtcTime->u32Year);
}
