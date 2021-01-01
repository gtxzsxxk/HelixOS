#ifndef ARCH_SUPP
#define ARCH_SUPP
#include "helix.h"

#define NVIC_INT_CTRL 0xE000ED04
#define NVIC_SYSPRI14 0xE000ED22
#define NVIC_PENDSV_PRI 0xFF
#define NVIC_PENDSVSET 0x10000000

void HL_Interrupt_DISABLE(void);
void HL_Interrupt_ENABLE(void);

void HL_System_INIT_Hidden(uint32_t Sys_stk);
uint32_t* HL_System_INIT_STACK(uint32_t* bottomStack,void (*Thread_Entry)(void));
void HL_PendSV_SET(void);
void HL_Thread_Switch(void);

#endif
