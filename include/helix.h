#ifndef HELIX_h_
#define HELIX_h_

#include "main.h"

/*
HelixOS designed by me
Transplant Tips:
OS is running on HAL Library
When making the OS on another platform
You need to confirm the configurations below
1 Confirm the System Type
2 Confirm the System Hard Settings
3 Make sure the SYSCLOCK SETTINGS and the timeslice
4 Disable the unused functions to reduce the code size
5 Go to pre_usrprvo_interface.c and rewrite all the functions
6 NVIC should be set to 2 bit 2 bit mode
7 check the arch_support.c on whether it works
8 The default timeslice is 1ms and Pulse Clock Frequence is suggested to be 1ms

if you only need the multi-thread function
please delete the C files and their header files except the follow ones
"arch_support.c" //Support the interface to the hardware
"pre_usrprvo_interface.c" //The file you need to rewrite
"library.c"  //The library which includes functions that may be used
"helix.c" //main file
"scheduler.c" //just like its name descibed

I recommend that you only delete C files
*/

/*引用C标准库*/
#include "string.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"

#define HL_HARDWARE_STM32F103//定义系统类型

#define HL_SMALL_STACK_SIZE 128
#define HL_MIDDLE_STACK_SIZE 256
#define HL_LARGE_STACK_SIZE 512
#define HL_HUGE_STACK_SIZE 1024

#define HL_BYTES_1KB 1024
#define HL_BYTES_2KB 2048
#define HL_BYTES_4KB 4096
#define HL_BYTES_8KB 8192


#define HL_SYS_FLAG_EMPTY 0x08
#define HL_SYS_FLAG_SYSTEM 0x04
#define HL_SYS_FLAG_USER 0x00

/*↓系统设置↓*/
#define HL_SYSINFO_CPUARCH "Cortex-M3" //CPU架构
#define HL_SYSINFO_VERSION "0.0.1 BETA" //版本
#define HL_LinkLIST_SIZE 256//最大任务数
#define HL_STATIC_TimeSlice 1 //ms 默认时间片

/*About the memory manager*/
#define HL_MEM_MANAGER_ENABLE //允许内存管理
	#ifdef HL_MEM_MANAGER_ENABLE
		#define HL_MEM_DEFAULT_MANAGEPOOL_SIZE 256 //默认内存池管理区大小
		#define HL_MEM_ALTERNATE_FUNCTION_ENABLE //使用hmalloc和hfree分配内存
	#endif
	
/*About the Terminal*/
#define HL_TERMINAL_IO_ENABLE //是否允许终端
	#ifdef HL_TERMINAL_IO_ENABLE
	#define HL_TERMINAL_BUFFER_SIZE 256 //终端缓冲区大小
	//#define HL_TERMINAL_MINI //是否使用MINI版终端（不包含文件系统功能，终端的阉割版）
	#endif
	
/*About the filesystem*/
#define HL_FILESYSTEM_ENABLE //允许文件系统存在
//Terminal loves filesystem,you'd better enable filesystem when using terminal

/*About the external ram*/
#define HL_EXTENDED_RAM_EXIST //存在外扩RAM
	#ifdef HL_EXTENDED_RAM_EXIST
	#define SRAM_SIZE 1048576
	#endif

/*About Time*/
#define HL_TIME_ENABLE //支持内置RTC

/*↑系统设置↑*/

//进程信息
typedef struct
{
	uint32_t* m_SP;//栈顶指针
	
	uint32_t priority;//优先级越小越优先 7355608为系统空闲任务
	
	const char* thread_name;
	
	uint8_t PID;
	
	uint32_t* bottom_Stack;// 栈底指针>m_SP
	
	uint32_t Ticks;//被调用的数量
	
	uint32_t TickSlice;//被调用的时间片数量
	
	float CPU_USAGE;//cpu使用率
	
	void (*Thread_Entry)(void); //函数入口 不能带参数
	
	uint8_t specialFlag;
	
	uint32_t delayCnt;
} HL_Thread;

extern uint32_t HL_SYS_INITED;
extern uint8_t HL_GLOBAL_TEMP_STRING[256];
extern void HL_WHILE(void);

//创建进程    结构体、4字节内的名字、函数入口、栈底、优先级
void HL_Thread_Create(HL_Thread* hl,const char* name,void (*Thread_Entry)(void),uint32_t* bottomStack,uint32_t priority);

//系统初始化，在初始化完线程后调用
void HL_System_Init(void);


/*引用系统内的文件*/
#include "library.h"
#include "scheduler.h"
#include "pre_usrprvo_interface.h"
#include "arch_support.h"

#ifdef HL_MEM_MANAGER_ENABLE
#include "mem_manager.h"
#endif

#ifdef HL_TIME_ENABLE
#include "htime.h"
#endif

#ifdef HL_TERMINAL_IO_ENABLE
#include "sys_app.h"
#include "Terminal.h"
#endif

#ifdef HL_FILESYSTEM_ENABLE
#include "file_system.h"
#endif


#endif
