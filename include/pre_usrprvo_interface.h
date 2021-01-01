#ifndef PRE_USRPRVO
#define PRE_USRPRVO

#include "helix.h"

#ifdef HL_HARDWARE_STM32F103

extern uint8_t huart_rev;

void HL_USERDEFINED_INITFUNC(void);//���û�����ģ���ϵͳ��ʼ������������ǰִ�еĺ���
void HL_System_Pulse_Enable(void); //ʹ������ʱ��
void HL_System_Pulse_Disable(void);//ʧ������ʱ��
void HL_System_Base_OUTPUT(uint8_t* dat);//ϵͳ�������

void HL_DAC_UpdateValue(uint16_t v);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif

#endif
