//架构支持文件，存放对CPU操作的代码
#include "arch_support.h"

#ifdef HL_HARDWARE_STM32F103

__asm void HL_Interrupt_DISABLE(void)
{
	CPSID I
	BX LR
}

__asm void HL_Interrupt_ENABLE(void)
{
	CPSIE I
	BX LR
}

uint32_t* HL_System_INIT_STACK(uint32_t* bottomStack,void (*Thread_Entry)(void))
{
	uint32_t* stk_ptr=(uint32_t*)((uint32_t)bottomStack&0xfffffff8u);//堆栈对齐
	*(--stk_ptr)=0x01000000UL;//xPSR
	*(--stk_ptr)=(uint32_t)Thread_Entry;//PC
	*(--stk_ptr)=(uint32_t)HL_WHILE;//LR
	*(--stk_ptr)=(uint32_t)0x12121212;//R12
	*(--stk_ptr)=(uint32_t)0x03030303;//R3
	*(--stk_ptr)=(uint32_t)0x02020202;//R2
	*(--stk_ptr)=(uint32_t)0x01010101;//R1
	*(--stk_ptr)=(uint32_t)0x00000000;//R0
	//未自动保存的
	*(--stk_ptr)=(uint32_t)0x11111111;//R11
	*(--stk_ptr)=(uint32_t)0x10101010;//R10
	*(--stk_ptr)=(uint32_t)0x09090909;//R9
	*(--stk_ptr)=(uint32_t)0x08080808;//R8
	*(--stk_ptr)=(uint32_t)0x07070707;//R7
	*(--stk_ptr)=(uint32_t)0x06060606;//R6
	*(--stk_ptr)=(uint32_t)0x05050505;//R5
	*(--stk_ptr)=(uint32_t)0x04040404;//R4
	//↑ 初始化堆栈
	return stk_ptr;
}

__asm void HL_System_INIT_Hidden(uint32_t Sys_stk)
{
	extern HL_SYS_INITED;
	CPSID I
	LDR R2,=NVIC_SYSPRI14//设PendSV优先级
	LDR R3,=NVIC_PENDSV_PRI
	STRB R3,[R2]
	
	LDR R2,=0x0
	MSR PSP,R2//修改PSP为0，作为第一次任务切换标志
	
	MSR MSP,R0//切换系统堆栈到指定的堆栈
	LDR R2,=0X01
	LDR R0,=HL_SYS_INITED
	STR R2,[R0]
	
	CPSIE I
	BX LR
	NOP
}

__asm void HL_PendSV_SET(void)
{
	LDR R0,=NVIC_INT_CTRL
	LDR R1,=NVIC_PENDSVSET
	STR R1,[R0]
	BX LR
	NOP
}

__asm void PendSV_Handler(void) //保存上文
{
	extern HL_currentThread;//currentSP的地址
	CPSID I //关中断
	
	MRS R0,PSP //R0=PSP
	CBZ R0,HL_Thread_Switch //如果为0，直接切换下文
	
	SUBS R0,R0,#0x20 //栈顶指针下减（CM3堆栈向下生长）32，入栈R4-R11
	STM R0,{R4-R11}
	
	LDR R1,=HL_currentThread//R1=HL_currentSP的地址
	LDR R1,[R1]// R1=HL_currentSP的值
	STR R0,[R1]//HL_currentSP的第一个元素的地址=R0
}

__asm void HL_Thread_Switch(void) //切换下文
{
	extern HL_nextThread;
	LDR R1,=HL_nextThread//R1=&HL_nextThread
	LDR R1,[R1]//R1=HL_nextThread
	LDR R0,[R1] //R0=下一个任务的栈地址
	
	LDM R0,{R4-R11} //出栈R4-R11
	ADDS R0,R0,#0x20 //R0自增32
	
	MSR PSP,R0 //更新PSP为下一个任务的栈地址
	ORR LR,LR,#0x04 //LR=LR|0x04 用户进程永远用PSP 权威手册42
	CPSIE I
	BX LR
}

#endif

/*((((((((((((((((((((CORTEX M0))))))))))))))))))))*/


#ifdef HL_HARDWARE_STM32F030

__asm void HL_Interrupt_DISABLE(void)
{
	CPSID I
}

__asm void HL_Interrupt_ENABLE(void)
{
	CPSIE I
}

uint32_t* HL_System_INIT_STACK(uint32_t* bottomStack,void (*Thread_Entry)(void))
{
	uint32_t* stk_ptr=(uint32_t*)((uint32_t)bottomStack&0xfffffff8u);//堆栈对齐
	*(--stk_ptr)=0x01000000UL;//xPSR
	*(--stk_ptr)=(uint32_t)Thread_Entry;//PC
	*(--stk_ptr)=(uint32_t)HL_WHILE;//LR
	*(--stk_ptr)=(uint32_t)0x12121212;//R12
	*(--stk_ptr)=(uint32_t)0x03030303;//R3
	*(--stk_ptr)=(uint32_t)0x02020202;//R2
	*(--stk_ptr)=(uint32_t)0x01010101;//R1
	*(--stk_ptr)=(uint32_t)0x00000000;//R0
	//未自动保存的
	*(--stk_ptr)=(uint32_t)0x11111111;//R11
	*(--stk_ptr)=(uint32_t)0x10101010;//R10
	*(--stk_ptr)=(uint32_t)0x09090909;//R9
	*(--stk_ptr)=(uint32_t)0x08080808;//R8
	*(--stk_ptr)=(uint32_t)0x07070707;//R7
	*(--stk_ptr)=(uint32_t)0x06060606;//R6
	*(--stk_ptr)=(uint32_t)0x05050505;//R5
	*(--stk_ptr)=(uint32_t)0x04040404;//R4
	//↑ 初始化堆栈
	return stk_ptr;
}

__asm void HL_System_INIT_Hidden(uint32_t Sys_stk)
{
	extern HL_SYS_INITED;
	CPSID I
	LDR R2,=NVIC_SYSPRI14//设PendSV优先级
	LDR R3,=NVIC_PENDSV_PRI
	STRB R3,[R2]
	
	LDR R2,=0x0
	MSR PSP,R2//修改PSP为0，作为第一次任务切换标志
	
	MSR MSP,R0//切换系统堆栈到指定的堆栈
	LDR R2,=0X01
	LDR R0,=HL_SYS_INITED
	STR R2,[R0]
	
	CPSIE I
	BX LR
	NOP
}

__asm void HL_PendSV_SET(void)
{
	LDR R0,=NVIC_INT_CTRL
	LDR R1,=NVIC_PENDSVSET
	STR R1,[R0]
	BX LR
	NOP
}

__asm void PendSV_Handler(void) //保存上文
{
	extern HL_currentThread;//currentSP的地址
	CPSID I //关中断
	
	MRS R0,PSP //R0=PSP
	CBZ R0,HL_Thread_Switch //如果为0，直接切换下文
	
	SUBS R0,R0,#0x20 //栈顶指针下减（CM3堆栈向下生长）32，入栈R4-R11
	STM R0,{R4-R11}
	
	LDR R1,=HL_currentThread//R1=HL_currentSP的地址
	LDR R1,[R1]// R1=HL_currentSP的值
	STR R0,[R1]//HL_currentSP的第一个元素的地址=R0
}

__asm void HL_Thread_Switch(void) //切换下文
{
	extern HL_nextThread;
	LDR R1,=HL_nextThread//R1=&HL_nextThread
	LDR R1,[R1]//R1=HL_nextThread
	LDR R0,[R1] //R0=下一个任务的栈地址
	
	LDM R0,{R4-R11} //出栈R4-R11
	ADDS R0,R0,#0x20 //R0自增32
	
	MSR PSP,R0 //更新PSP为下一个任务的栈地址
	ORR LR,LR,#0x04 //LR=LR|0x04 用户进程永远用PSP 权威手册42
	CPSIE I
	BX LR
}

#endif
