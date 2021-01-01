#include "htime.h"

uint8_t HL_TIME_WEEKSTR[][10]={"Sunday","Monday"
														,"Tuesday","Wednesday",
														"Thursday","Friday","Saturday"};
																										// 01234567
														uint8_t HL_TIME_TIMESTR[]="00:00:00";

void HL_TIME_SetTIME(HL_TIME* ht)
{
	RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  
  sTime.Hours = ht->hour;
  sTime.Minutes = ht->minute;
  sTime.Seconds = ht->second;

  HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	
  DateToUpdate.WeekDay = ht->week;
  DateToUpdate.Month = ht->month;
  DateToUpdate.Date = ht->day;
  DateToUpdate.Year = ht->year%100;

  HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN);
}

void HL_TIME_GetTIME(HL_TIME* ht)
{
	RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};
	HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&DateToUpdate,RTC_FORMAT_BIN);
	ht->day=DateToUpdate.Date;
	ht->month=DateToUpdate.Month;
	ht->year=DateToUpdate.Year+2000;
	ht->week=DateToUpdate.WeekDay;
	
	ht->second=sTime.Seconds;
	ht->minute=sTime.Minutes;
	ht->hour=sTime.Hours;
	
	HL_TIME_TIMESTR[0]=ht->hour/10+'0';
	HL_TIME_TIMESTR[1]=ht->hour%10+'0';
	HL_TIME_TIMESTR[3]=ht->minute/10+'0';
	HL_TIME_TIMESTR[4]=ht->minute%10+'0';
	HL_TIME_TIMESTR[6]=ht->second/10+'0';
	HL_TIME_TIMESTR[7]=ht->second%10+'0';
}

