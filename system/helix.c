#include "helix.h"

uint32_t HL_SYS_INITED=0;
uint8_t HL_GLOBAL_TEMP_STRING[256];

//必须被静态分配
uint32_t HL_System_Stack[HL_LARGE_STACK_SIZE];

#ifdef HL_EXTENDED_RAM_EXIST
uint32_t* h_sram=(uint32_t*)0x68000000;
#endif

void HL_WHILE(void)
{
	
	while(1);
}

void HL_Thread_Create(HL_Thread* hl,const char* name,void (*Thread_Entry)(void),uint32_t* bottomStack,uint32_t priority)
{
	hl->m_SP=HL_System_INIT_STACK(bottomStack,Thread_Entry);
	hl->bottom_Stack=bottomStack;
	hl->priority=priority;
	hl->Ticks=0;
	hl->TickSlice=0;
	hl->Thread_Entry=Thread_Entry;
	hl->thread_name=name;
	hl->PID=HL_LinkLIST_LENG;
	hl->delayCnt=0;
	HL_LinkLIST_Insert((uint32_t)hl);
}

#ifdef HL_MEM_MANAGER_ENABLE

void HL_Thread_Create_DYNAMIC(HL_Thread* hl,const char* name,void (*Thread_Entry)(void),uint32_t STACKSIZE,uint32_t priority)
{
	uint32_t* stack=hmalloc(STACKSIZE*4);
	hl->m_SP=HL_System_INIT_STACK(stack+STACKSIZE-1,Thread_Entry);
	hl->bottom_Stack=stack+STACKSIZE-1;
	hl->priority=priority;
	hl->Ticks=0;
	hl->TickSlice=0;
	hl->Thread_Entry=Thread_Entry;
	hl->thread_name=name;
	hl->PID=HL_LinkLIST_LENG;
	hl->delayCnt=0;
	HL_LinkLIST_Insert((uint32_t)hl);
}

#endif

void HL_System_Init()
{
	#ifdef HL_EXTENDED_RAM_EXIST
	HL_Mem_AssignPool(&DefaultPool,(uint32_t)h_sram,SRAM_SIZE,HL_MEM_DEFAULT_MANAGEPOOL_SIZE);
		//为终端缓存区分配空间
		#ifdef HL_TERMINAL_IO_ENABLE
		HL_System_Terminal_INPUT_BUFFER=hmalloc(HL_TERMINAL_BUFFER_SIZE);
		HL_System_Terminal_INPUT_TEMPBUFFER=hmalloc(HL_TERMINAL_BUFFER_SIZE);
		HL_System_Terminal_INPUT_DATA=hmalloc(64); //64
		HL_System_Terminal_INPUT_CMD=hmalloc(16); //16
		//申请内存后要清理
		memset(HL_System_Terminal_INPUT_BUFFER,0,HL_TERMINAL_BUFFER_SIZE);
		memset(HL_System_Terminal_INPUT_TEMPBUFFER,0,HL_TERMINAL_BUFFER_SIZE);
		memset(HL_System_Terminal_INPUT_DATA,0,64);
		memset(HL_System_Terminal_INPUT_CMD,0,16);
		#endif
	#endif
	
	#ifdef HL_TERMINAL_IO_ENABLE
	
	HL_Thread_Create(&hl_terminal_thread,"CMD",HL_Terminal_Entry,&HL_System_Terminal_Stack[HL_BYTES_4KB-1],1);
	//对速度访问要求高的线程，栈不允许使用外部SRAM
	HL_Terminal_Write("\r\n#########\r\nHelix RealTime Operation System\r\n#########\r\n");
	HL_Terminal_Write("Yours faithfully,helixos\r\n");
	HL_APP_SYSINFO();
	#endif
	
	HL_Interrupt_DISABLE();
	HL_LinkLIST_Init();
	HL_System_Pulse_Enable();
	HL_System_INIT_Hidden((uint32_t)&HL_System_Stack[HL_LARGE_STACK_SIZE-1]);
	
	HL_WHILE();//等待调度器执行第一次调度
}


