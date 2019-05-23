#ifndef __RTC_H__
#define __RTC_H__


typedef struct rtc_time_s{
	U32 u32Sec;  //[0, 59]
	U32 u32Min;  //[0, 59]
	U32 u32Hour; //[0, 23]
	U32 u32DayWeek; //[1, 7]
	U32 u32Day;  //[1, 31]
	U32 u32Mon;  //[1, 12]
	U32 u32Year; //[0, 99] ,the start year is 2000
}rtc_time_t;


//Brief: get rtc time
//[In]:  NA
//[Out]: pstRtcTime  rtc time struct pointer
//Ret: 0 success, < 0 failed
S32 rtc_get_time(rtc_time_t *pstRtcTime);


//Brief:set rtc time
//[In]: pstRtcTime  rtc time pointer
//[Out]:NA
//Ret:0 success, < 0 failed
S32 rtc_set_time(rtc_time_t *pstRtcTime);


//Brief: reset rtc time
//[In]:NA
//[Out]:NA
//Ret: 0 success, < 0 failed
S32 rtc_reset_time(void);


#endif //__RTC_H__
