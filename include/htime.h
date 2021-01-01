#ifndef HL_HTIME
#define HL_HTIME
#include "helix.h"

#ifdef HL_TIME_ENABLE

typedef struct
{
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t week;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} HL_TIME;

extern uint8_t HL_TIME_WEEKSTR[][10];
extern uint8_t HL_TIME_TIMESTR[];
void HL_TIME_SetTIME(HL_TIME* ht);
void HL_TIME_GetTIME(HL_TIME* ht);
#endif

#endif
