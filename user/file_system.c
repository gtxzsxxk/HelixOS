#include "file_system.h"

/*
FATFS以及SDIO的坑：
开发板无法使用4B WideBUS
要在HAL_SD_ConfigWideBusOperation初始化时修改为1b
分频系数不能=0
操作SDIO的线程栈不能设置为外部SRAM，因为速度不够会导致SDIO OVERRUN
*/


uint32_t HL_FS_BW=0;//不知道这个变量干吗用的

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



