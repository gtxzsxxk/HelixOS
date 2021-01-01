#include "sys_app.h"

void HL_APP_SYSINFO(void)
{
	sprintf((char*)HL_GLOBAL_TEMP_STRING,"\r\n[SYSTEM INFORMATION]\r\n#[CPU ARCHITECTURE]=%s\r\n#[SYSTEM VERSION]=%s\r\n#[MAX THREAD]=%d\r\n\r\n",
	HL_SYSINFO_CPUARCH,HL_SYSINFO_VERSION,HL_LinkLIST_SIZE);
	HL_Terminal_Write(HL_GLOBAL_TEMP_STRING);
	memset(HL_GLOBAL_TEMP_STRING,0,sizeof(HL_GLOBAL_TEMP_STRING));
}

void HL_APP_GetTime(void)
{
	#ifdef HL_TIME_ENABLE
	HL_TIME ht;
	HL_TIME_GetTIME(&ht);
	sprintf((char*)HL_System_Terminal_INPUT_TEMPBUFFER,"[HelixOS Inside RTC]\r\n#Date %d/%d/%d\r\n#WeekDay %s\r\n#Time %s"
	,ht.year,ht.month,ht.day,HL_TIME_WEEKSTR[ht.week],HL_TIME_TIMESTR);
	HL_Terminal_Write(HL_System_Terminal_INPUT_TEMPBUFFER);
	memset(HL_System_Terminal_INPUT_TEMPBUFFER,0,HL_TERMINAL_BUFFER_SIZE);
	#endif
}

void HL_APP_SetTime(void)
{
	#ifdef HL_TIME_ENABLE
	HL_TIME ht={0};
	
	#ifdef HL_MEM_MANAGER_ENABLE
	uint8_t* tempInputBuffer=hmalloc(128);
	#else
	uint8_t tempInputBuffer[128];
	#endif
	
	HL_Terminal_Write("FORMAT:\r\nyyyy/M/D week(0-6) HH:mm:ss\r\nDATA INPUT:");
	
	memset(tempInputBuffer,0,128);
	HL_Terminal_ReadLine(tempInputBuffer);
	
	sscanf((const char*)tempInputBuffer,"%hd/%hhu/%hhu %hhu %hhu:%hhu:%hhu",&ht.year,
	&ht.month,&ht.day,&ht.week,&ht.hour,&ht.minute,&ht.second);
	memset(tempInputBuffer,0,128);
	HL_TIME_SetTIME(&ht);
		
	HL_TIME_GetTIME(&ht);
	sprintf((char*)tempInputBuffer,"[HelixOS Inside RTC]\r\n#Date %d/%d/%d\r\n#WeekDay %s\r\n#Time %s"
	,ht.year,ht.month,ht.day,HL_TIME_WEEKSTR[ht.week],HL_TIME_TIMESTR);
	HL_Terminal_Write(tempInputBuffer);
	
	#ifdef HL_MEM_MANAGER_ENABLE
	hfree(tempInputBuffer);
	#endif
	
	#endif
}

void HL_APP_GetCPURATE(void)
{
	uint8_t PID;
	sscanf((const char*)HL_System_Terminal_INPUT_DATA,"get-cpurate %hhu",&PID);
	HL_Thread* ht=HL_LinkLIST_ACCESS_BYPID(PID);
		
	float rate=((float)ht->Ticks/(float)(ht->TickSlice*HL_STATIC_TimeSlice))*1000;
	sprintf((char*)HL_System_Terminal_INPUT_TEMPBUFFER,"Thread %hhu CPU USAGE RATE: %0.2lf per",PID,rate);
	HL_Terminal_Write(HL_System_Terminal_INPUT_TEMPBUFFER);
	memset(HL_System_Terminal_INPUT_TEMPBUFFER,0,HL_TERMINAL_BUFFER_SIZE);
}

void HL_APP_GetThread(void)
{
	float totalrate=0;
	HL_Terminal_Write("[HELIX TASKMANAGER VERSION 0.0.1]\r\n");
	HL_Terminal_Write("PID\tName\tTicks\tTickSlice\tCPU\tPriority\r\n");
	for(int i=HL_LinkLIST_LENG-1;i>=0;i--)
	{
		HL_Thread* ht=HL_LinkLIST_ACCESS(HL_LinkLIST[i]);
		float rate=((float)ht->Ticks/(float)(ht->TickSlice*HL_STATIC_TimeSlice))*1000;
		sprintf((char*)HL_System_Terminal_INPUT_TEMPBUFFER,"%d\t%s\t%d\t%d\t%0.2lf\t%d\r\n"
			,ht->PID,ht->thread_name,ht->Ticks,ht->TickSlice,rate,ht->priority);
		totalrate+=rate;
		HL_Terminal_Write(HL_System_Terminal_INPUT_TEMPBUFFER);
		memset(HL_System_Terminal_INPUT_TEMPBUFFER,0,HL_TERMINAL_BUFFER_SIZE);
	}
	sprintf((char*)HL_System_Terminal_INPUT_TEMPBUFFER,"[CPU TOTAL USAGE RATE] %0.2lf\r\n",totalrate);
	HL_Terminal_Write(HL_System_Terminal_INPUT_TEMPBUFFER);
	memset(HL_System_Terminal_INPUT_TEMPBUFFER,0,HL_TERMINAL_BUFFER_SIZE);
}

