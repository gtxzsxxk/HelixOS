#ifndef HLSCHE
#define HLSCHE
#include "helix.h"

#define HL_SCHEDULER_START HL_System_Pulse_Enable
#define HL_SCHEDULER_STOP HL_System_Pulse_Disable

extern uint32_t HL_LinkLIST[HL_LinkLIST_SIZE];
extern uint32_t HL_LinkLIST_LENG;//������
extern uint32_t HL_LinkLIST_PTR;//��ǰִ�е�����

extern HL_Thread* HL_currentThread;
extern HL_Thread* HL_nextThread;

extern uint32_t HL_SCHEDULER_PULSE;

void HL_LinkLIST_Insert(uint32_t Thread_ADDR);
void HL_LinkLIST_Remove(uint32_t Thread_ADDR);
HL_Thread* HL_LinkLIST_ACCESS(uint32_t addr);
HL_Thread* HL_LinkLIST_ACCESS_BYPID(uint8_t PID);
void HL_LinkLIST_SWAP(uint32_t ptr1,uint32_t ptr2);
void HL_LinkLIST_SORT(void);

void HL_Thread_Delay(HL_Thread* ht,uint32_t period);//ʵ��������ʱ

void HL_LinkLIST_Init(void);//��������ʼ��

void HL_LinkLIST_Refresh(void);//ˢ�µ�������¼

void HL_LinkLIST_Schedule(void);//ִ�е��� �������������

#endif
