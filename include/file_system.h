#ifndef HL_FILESYS
#define HL_FILESYS



#include "helix.h"
#include "fatfs.h"
#include "main.h"
#ifdef HL_FILESYSTEM_ENABLE

typedef struct
{
	FIL SDFile;
	uint8_t* name;
	uint8_t* path;
	uint8_t* bufferInMem;
	uint32_t size_t;
	uint32_t fptr;
}File;

void HL_FS_Init(void);

void HL_FS_AppendFile(uint8_t* path,uint8_t* content);//追加写文件

void HL_FS_LOG(uint8_t * logstr);

void HL_FS_GetFiles(void);
void HL_FS_ShowFilesAsTXT(uint8_t* filepath);
void HL_FS_WriteFile_CTRCMD(void); //接管CMD，对文件读写操作

#endif

#endif
