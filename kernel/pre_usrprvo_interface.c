//用户需要预先定义的操作系统接口实现函数
#include "pre_usrprvo_interface.h"

#ifdef HL_HARDWARE_STM32F103


uint8_t huart_rev;
void HL_USERDEFINED_INITFUNC(void)
{
	
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==htim1.Instance)
	{
		HL_System_Pulse_Enable();
		HL_LinkLIST_Schedule();
		HL_SCHEDULER_PULSE++;
	}
}

void HL_System_Pulse_Enable(void)
{
	HAL_TIM_Base_Start_IT(&htim1);
}

void HL_System_Pulse_Disable(void)
{
	HAL_TIM_Base_Stop_IT(&htim1);
}

void HL_System_Base_OUTPUT(uint8_t* dat)
{
	#ifdef HL_TERMINAL_IO_ENABLE
	HAL_UART_Transmit(&huart1,dat,strlen((const char*)dat),100);
	#endif
}



void HL_DAC_UpdateValue(uint16_t v)
{
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_8B_R,v);
	HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==huart1.Instance)
	{
		HAL_UART_Receive_IT(&huart1,&huart_rev,1);
		HL_System_Terminal_INPUT_BUFFER[HL_System_Terminal_INPUT_PTR++]=huart_rev;
		if(HL_System_Terminal_INPUT_PTR==(uint8_t)HL_TERMINAL_BUFFER_SIZE)
			HL_System_Terminal_INPUT_PTR=0;
		if(!HL_Terminal_EnterPassword)
			USART1->DR=huart_rev;
	}
}

#endif
