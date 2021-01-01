#ifndef HL_MEM_MANAGER
#define HL_MEM_MANAGER
#include "Helix.h"

#ifdef HL_MEM_MANAGER_ENABLE

typedef struct
{
	uint32_t m_begin;
	uint32_t m_end;
	uint32_t Size;//字节
	uint32_t Used;//是否被使用
} HL_Mem_BLOCK;//16 bytes

typedef struct
{
	uint32_t pool;//整个内存池
	uint32_t size;//bytes
	uint32_t managePool_size;//内存管理池的大小（支持多少个16 byte内存管理池）
	uint32_t mem_begin;//可分配内存起始地址
	uint32_t block_len;//内存管理块大小
	uint32_t free_memptr;//可分配自由内存起始地址
} HL_Mem_Pool; 

#ifdef HL_MEM_ALTERNATE_FUNCTION_ENABLE //使用hmalloc和hfree分配内存

extern HL_Mem_Pool DefaultPool;
#define hmalloc(x) HL_Mem_Allocate(&DefaultPool,x)
#define hfree(x) HL_Mem_Free((uint32_t)x,&DefaultPool)

#endif
//内存池结构体、内存地址、内存大小、内存管理区大小
void HL_Mem_AssignPool(HL_Mem_Pool* hp,uint32_t pool,uint32_t size,uint32_t mpsize);
void* HL_Mem_Allocate(HL_Mem_Pool* hp,uint32_t size);
void HL_Mem_Free(uint32_t m_begin,HL_Mem_Pool* hp);
uint8_t HL_Mem_IsOverflow(uint32_t mem_begin,uint32_t size,HL_Mem_Pool* hp);

#endif

#endif
