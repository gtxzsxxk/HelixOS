#ifndef HL_MEM_MANAGER
#define HL_MEM_MANAGER
#include "Helix.h"

#ifdef HL_MEM_MANAGER_ENABLE

typedef struct
{
	uint32_t m_begin;
	uint32_t m_end;
	uint32_t Size;//�ֽ�
	uint32_t Used;//�Ƿ�ʹ��
} HL_Mem_BLOCK;//16 bytes

typedef struct
{
	uint32_t pool;//�����ڴ��
	uint32_t size;//bytes
	uint32_t managePool_size;//�ڴ����صĴ�С��֧�ֶ��ٸ�16 byte�ڴ����أ�
	uint32_t mem_begin;//�ɷ����ڴ���ʼ��ַ
	uint32_t block_len;//�ڴ������С
	uint32_t free_memptr;//�ɷ��������ڴ���ʼ��ַ
} HL_Mem_Pool; 

#ifdef HL_MEM_ALTERNATE_FUNCTION_ENABLE //ʹ��hmalloc��hfree�����ڴ�

extern HL_Mem_Pool DefaultPool;
#define hmalloc(x) HL_Mem_Allocate(&DefaultPool,x)
#define hfree(x) HL_Mem_Free((uint32_t)x,&DefaultPool)

#endif
//�ڴ�ؽṹ�塢�ڴ��ַ���ڴ��С���ڴ��������С
void HL_Mem_AssignPool(HL_Mem_Pool* hp,uint32_t pool,uint32_t size,uint32_t mpsize);
void* HL_Mem_Allocate(HL_Mem_Pool* hp,uint32_t size);
void HL_Mem_Free(uint32_t m_begin,HL_Mem_Pool* hp);
uint8_t HL_Mem_IsOverflow(uint32_t mem_begin,uint32_t size,HL_Mem_Pool* hp);

#endif

#endif
