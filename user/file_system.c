#include "file_system.h"

/*
FATFS�Լ�SDIO�Ŀӣ�
�������޷�ʹ��4B WideBUS
Ҫ��HAL_SD_ConfigWideBusOperation��ʼ��ʱ�޸�Ϊ1b
��Ƶϵ������=0
����SDIO���߳�ջ��������Ϊ�ⲿSRAM����Ϊ�ٶȲ����ᵼ��SDIO OVERRUN
*/


uint32_t HL_FS_BW=0;//��֪��������������õ�

void HL_FS_Init(void)
{
	retSD = f_mount(&SDFatFS, "", 0);
	if(retSD==0)
	{
		HL_Terminal_Write("FILE SYSTEM INSTALLED OK;\r\n");
	}
	else
	{
		HL_Terminal_Write("FAIL TO INSTALL FILESYSTEM;\r\n");
		HL_WHILE();
	}
	
	HL_TIME ht;
	HL_TIME_GetTIME(&ht);
	
	#ifdef HL_MEM_MANAGER_ENABLE
	uint8_t* tempInputBuffer=hmalloc(48);
	#else
	uint8_t tempInputBuffer[48];
	#endif
	
	memset(tempInputBuffer,0,48);
	
	HL_TIME_GetTIME(&ht);
	sprintf((char*)tempInputBuffer,"%d/%d/%d %s -> "
	,ht.year,ht.month,ht.day,HL_TIME_TIMESTR);
	HL_FS_LOG(tempInputBuffer);
	HL_FS_LOG("Helix SYSTEM Initialize Successfully.\r\n");
	
	#ifdef HL_MEM_MANAGER_ENABLE
	hfree(tempInputBuffer);
	#endif
}

void HL_FS_AppendFile(uint8_t* path,uint8_t* content)
{
	retSD = f_open(&SDFile,(const char*)path, FA_OPEN_ALWAYS | FA_WRITE);
	uint32_t size_F=f_size(&SDFile);
	f_lseek(&SDFile,size_F);
	retSD=f_write(&SDFile,content,strlen((const char*)content),&HL_FS_BW);
	f_close(&SDFile);
}

void HL_FS_LOG(uint8_t * logstr)
{
	HL_FS_AppendFile((uint8_t*)"helix.log",logstr);
}



