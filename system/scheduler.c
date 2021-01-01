//������
#include "scheduler.h"

uint32_t HL_LinkLIST[HL_LinkLIST_SIZE];
uint32_t HL_LinkLIST_LENG=0;//������
uint32_t HL_LinkLIST_PTR=0;//��ǰִ�е�����

HL_Thread* HL_currentThread;
HL_Thread* HL_nextThread;

/*��������*/
uint32_t HL_EMPTYTASK_STACK[HL_SMALL_STACK_SIZE];
HL_Thread HL_EMPTYTASK;

uint32_t HL_SCHEDULER_PULSE=0;//���������� ��ʱ����pre_usrprvo_interface.c����

void HL_LinkLIST_Insert(uint32_t Thread_ADDR)
{
	if(HL_LinkLIST_LENG==HL_LinkLIST_SIZE-1)
	{
		//error
		return;
	}
	if(HL_LinkLIST_LENG==0)
	{
		HL_LinkLIST[HL_LinkLIST_LENG]=Thread_ADDR;
		HL_LinkLIST_LENG++;
		return;
	}
	
	HL_LinkLIST[HL_LinkLIST_LENG++]=Thread_ADDR;
	
	HL_LinkLIST_SORT();
}

void HL_LinkLIST_Remove(uint32_t Thread_ADDR)
{
	uint8_t i;
	for(i=0;i<HL_LinkLIST_LENG;i++)
	{
		if(HL_LinkLIST[i]==Thread_ADDR)
		{
			HL_LinkLIST[i]=0;
			HL_LinkLIST_LENG--;
			HL_LinkLIST_SORT();
			return;
		}
	}
	HL_LinkLIST_SORT();
}

HL_Thread* HL_LinkLIST_ACCESS(uint32_t addr)
{
	HL_Thread* thr=(HL_Thread*)addr;
	return thr;
}

HL_Thread* HL_LinkLIST_ACCESS_BYPID(uint8_t PID)
{
	HL_Thread* ht=NULL;
	for(uint8_t i=0;i<HL_LinkLIST_LENG;i++)
	{
		ht=HL_LinkLIST_ACCESS(HL_LinkLIST[i]);
		if(ht->PID==PID) return ht;
	}
	return NULL;
}

void HL_LinkLIST_SWAP(uint32_t ptr1,uint32_t ptr2)
{
	uint32_t temp=HL_LinkLIST[ptr1];
	HL_LinkLIST[ptr1]=HL_LinkLIST[ptr2];
	HL_LinkLIST[ptr2]=temp;
}

void HL_LinkLIST_SORT(void)
{
	uint8_t i,j;
	HL_Thread* temp1;
	HL_Thread* temp2;
	
	for(i=0;i<HL_LinkLIST_LENG;i++)
	{
		for(j=i+1;j<HL_LinkLIST_LENG;j++)
		{
			temp1=HL_LinkLIST_ACCESS(HL_LinkLIST[i]);
			temp2=HL_LinkLIST_ACCESS(HL_LinkLIST[j]);
			if(temp2->priority<temp1->priority)
			HL_LinkLIST_SWAP(i,j);
		}
	}
}

void HL_Thread_Delay(HL_Thread* ht,uint32_t period)
{
	ht->delayCnt=period+HL_SCHEDULER_PULSE;
	HL_LinkLIST_Schedule();//�������
}

void HL_LinkLIST_Init(void)
{
	HL_Thread_Create(&HL_EMPTYTASK,"EMPTY",HL_WHILE,&HL_EMPTYTASK_STACK[HL_SMALL_STACK_SIZE-1],7355608);
	HL_EMPTYTASK.specialFlag=10;//means empty task
	HL_currentThread=HL_LinkLIST_ACCESS(HL_LinkLIST[HL_LinkLIST_LENG-1]);
	HL_LinkLIST_PTR=HL_LinkLIST_LENG-1;
	//��ǰ����Ϊ���һ���������һ�������ǿ���������ΪҪ�л�����һ������ȥ
	HL_nextThread=HL_LinkLIST_ACCESS(HL_LinkLIST[0]);//��һ������Ϊ��һ������
}

void HL_LinkLIST_Refresh(void)
{
	HL_currentThread=HL_LinkLIST_ACCESS(HL_LinkLIST[HL_LinkLIST_PTR]);
	//�ӵ�ǰ���̿�ʼ�������
	while(1)
	{
		if(HL_LinkLIST_PTR>=HL_LinkLIST_LENG-1) 
			HL_LinkLIST_PTR=0;
		else 
			HL_LinkLIST_PTR++;
		
		//HL_nextThread=HL_LinkLIST_ACCESS(HL_LinkLIST[HL_LinkLIST_PTR]);
		HL_Thread* temp_next=HL_LinkLIST_ACCESS(HL_LinkLIST[HL_LinkLIST_PTR]);
		if(temp_next==HL_currentThread)
		{
			//û�п���ִ�е�����
			HL_nextThread=&HL_EMPTYTASK;//ִ�п�������
			HL_LinkLIST_PTR=HL_LinkLIST_LENG-1;
			break;// ���ѻ�����
		}
		
		if(temp_next!=&HL_EMPTYTASK&&temp_next->delayCnt<=HL_SCHEDULER_PULSE)
		{
			HL_nextThread=temp_next;
			HL_nextThread->delayCnt=0;
			break;
		}
	}
	//������̵�delayCnt���ڵ�ǰ�����������������ң�����һ������ڵĽ��̣���֤��û�п���ִ�е�����ִ�п�������
	//ֻҪdelayCntС�ڵ�ǰ����������ζ�ű���Ҫִ��������̣�����ռ�����
}

void HL_LinkLIST_Schedule(void)
{
	HL_LinkLIST_Refresh();
	HL_nextThread->TickSlice++;
	if(HL_LinkLIST_LENG>1&&HL_SYS_INITED)
		HL_PendSV_SET();
}
