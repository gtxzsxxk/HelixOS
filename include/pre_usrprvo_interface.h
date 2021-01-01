#ifndef PRE_USRPRVO
#define PRE_USRPRVO

#include "helix.h"

#ifdef HL_HARDWARE_STM32F103

extern uint8_t huart_rev;

void HL_USERDEFINED_INITFUNC(void);//由用户定义的，在系统初始化调度器加载前执行的函数
void HL_System_Pulse_Enable(void); //使能心跳时钟
void HL_System_Pulse_Disable(void);//失能心跳时钟
void HL_System_Base_OUTPUT(uint8_t* dat);//系统基本输出

void HL_DAC_UpdateValue(uint16_t v);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif

#endif
