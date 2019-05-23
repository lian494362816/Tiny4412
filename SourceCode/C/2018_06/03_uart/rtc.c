#include "reg.h"
#include "datatype.h"
#include "bcd.h"
#include "rtc.h"

/* typedef struct rtc_time_s{ */
	/* U32 u32Sec;  //[0, 59] */
	/* U32 u32Min;  //[0, 59] */
	/* U32 u32Hour; //[0, 23] */
	/* U32 u32DayWeek; //[1, 7] */
	/* U32 u32Day;  //[1, 31] */
	/* U32 u32Mon;  //[1, 12] */
	/* U32 u32Year; //0, 99] ,the start year is 2000 */
/* }rtc_time_t; */

static PRINT_T printf = PRINT_ADDR;

S32 rtc_get_time(rtc_time_t *pstRtcTime)
{
	S32 s32Ret = SUCCESS;

	if (NULL == pstRtcTime)
	{
		printf("NULL Pointer\n");
		return FAIL;
	}

	pstRtcTime->u32Sec = bcd2bin(BCDSEC);
	pstRtcTime->u32Min = bcd2bin(BCDMIN);
	pstRtcTime->u32Hour = bcd2bin(BCDHOUR);
	pstRtcTime->u32DayWeek = bcd2bin(BCDDAYWEEK);
	pstRtcTime->u32Day = bcd2bin(BCDDAY);
	pstRtcTime->u32Mon = bcd2bin(BCDMON);
	pstRtcTime->u32Year = bcd2bin(BCDYEAR);
	pstRtcTime->u32Year += 2000;

	//read again, because "one second deviation"
	if (0 == pstRtcTime->u32Sec)
	{
		pstRtcTime->u32Min = bcd2bin(BCDMIN);
		pstRtcTime->u32Hour = bcd2bin(BCDHOUR);
		pstRtcTime->u32DayWeek = bcd2bin(BCDDAYWEEK);
		pstRtcTime->u32Day = bcd2bin(BCDDAY);
		pstRtcTime->u32Mon = bcd2bin(BCDMON);
		pstRtcTime->u32Year = bcd2bin(BCDYEAR);
		pstRtcTime->u32Year += 2000;
	}

	return s32Ret;
}

S32 rtc_set_time(rtc_time_t *pstRtcTime)
{
	if (NULL == pstRtcTime)
	{
		printf("NULL Pointer\n");
		return FAIL;
	}

	RTCCON |= 0x1;


	if (pstRtcTime->u32Min > 59)
	{
		printf("Minute out of range:%d\n", pstRtcTime->u32Min);
		return FAIL;
	}

	if (pstRtcTime->u32Hour > 23)
	{
		printf("Hour out of range:%d\n", pstRtcTime->u32Hour);	
		return FAIL;
	}

	if (pstRtcTime->u32DayWeek > 7)
	{
		printf("DayWeek out of range:%d\n", pstRtcTime->u32DayWeek);
		return FAIL;
	}

	if (pstRtcTime->u32Mon > 12)
	{
		printf("Mon out of range:%d\n", pstRtcTime->u32Mon);
		return FAIL;
	}


	pstRtcTime->u32Year -= 2000;
	if (pstRtcTime->u32Year > 99)
	{
		printf("Year out of range:%d\n", pstRtcTime->u32Year);
		return FAIL;
	}
	// 1 3 5 7 8 10 12: 31 day
	// 4 6 9 11: 30 day
	// 2: 28 or 29
	switch(pstRtcTime->u32Mon)
	{
		case 4:
		case 6:
		case 9:
		case 11:
			if (pstRtcTime->u32Day > 30)
			{
				printf("Mon:%d, Max day is 30\n", pstRtcTime->u32Mon);	
				return FAIL;
			}
		break;

		case 2:
			//leap year
			if (0 == (pstRtcTime->u32Year % 4))
			{
				if (pstRtcTime->u32Day > 29)
				{
					printf("Leap Year, Mon:%d, Max day is 29\n", pstRtcTime->u32Mon);
					return FAIL;
				}
			}
			else 
			{
				if (pstRtcTime->u32Day > 28)
				{
					printf("normal year, Mon:%d, Max day is 28\n", pstRtcTime->u32Mon);
					return FAIL;
				}
			}
		break;

		default:
			if (pstRtcTime->u32Day > 31)
			{
				printf("Day out of range:%d\n", pstRtcTime->u32Day);
				return FAIL;
			}
		break;
	}


	if (pstRtcTime->u32Year > 2000)
	{
		pstRtcTime->u32Year -= 2000;
	}

	BCDSEC = bin2bcd(pstRtcTime->u32Sec);
	BCDMIN = bin2bcd(pstRtcTime->u32Min);
	BCDHOUR = bin2bcd(pstRtcTime->u32Hour);
	BCDDAYWEEK = bin2bcd(pstRtcTime->u32DayWeek);
	BCDDAY = bin2bcd(pstRtcTime->u32Day);
	BCDMON = bin2bcd(pstRtcTime->u32Mon);
	BCDYEAR = bin2bcd(pstRtcTime->u32Year);

	RTCCON &= ~0x1;

	return SUCCESS;
}

S32 rtc_reset_time(void)
{
	RTCCON |= 0x1;	

	BCDSEC = 0;
	BCDMIN = 0;
	BCDHOUR = 0;
	BCDDAYWEEK = 0;
	BCDDAY = 0;
	BCDMON = 0;
	BCDYEAR = 0;

	RTCCON &= ~0x1;

	return SUCCESS;
}

