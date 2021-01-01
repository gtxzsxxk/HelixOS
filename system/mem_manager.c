#include "mem_manager.h"

HL_Mem_Pool DefaultPool;

void HL_Mem_AssignPool(HL_Mem_Pool* hp,uint32_t pool,uint32_t size,uint32_t mpsize)
{
	hp->block_len=0;
	hp->managePool_size=mpsize;
	hp->size=size;
	hp->pool=pool;
	hp->mem_begin=hp->pool+hp->managePool_size;
	hp->free_memptr=hp->mem_begin;
}

void HL_Mem_StoreBLOCK(HL_Mem_BLOCK hb,HL_Mem_Pool* hp)
{
	uint32_t addr=hp->pool+(hp->block_len++)*16;
	HL_Mem_BLOCK* hptr=(HL_Mem_BLOCK*)addr;
	memcpy(hptr,&hb,sizeof(HL_Mem_BLOCK));
}

void* HL_Mem_Allocate(HL_Mem_Pool* hp,uint32_t size)
{
	if(hp->block_len==0)
	{
		if(HL_Mem_IsOverflow(hp->free_memptr,size,hp))//没有分配过内存，并且大小不会溢出
		{
			HL_Mem_BLOCK hb;
			hb.m_begin=hp->free_memptr;//设置块起点
			hb.m_end=hp->free_memptr+size-1;//设置块终点
			hb.Size=size;//设置块大小
			hb.Used=1;//标志使用
			
			HL_Mem_StoreBLOCK(hb,hp);
			hp->free_memptr+=size;//内存池自由内存+=此次分配大小
			return (uint32_t*)hb.m_begin;//返回起始地址
		}
	}
	else if(hp->block_len>0)
	{
		//注意这里是+2，因为可能在下面的程序中会分配两个块
		if(hp->block_len+2>hp->managePool_size) return NULL;//如果块满了就无法分配
		else
		{
			for(uint32_t i=0;i<hp->block_len;i++)
			{
				HL_Mem_BLOCK* hb=(HL_Mem_BLOCK*)(hp->pool+i*sizeof(HL_Mem_BLOCK));
				if(hb->Used==0&&hb->Size>=size)//寻找被舍弃且大小适合的块
				{
					hb->Used=1;//置位
					//判断是否有多余出的内存，将其创建为一个新块
					if(hb->Size-size>1)//至少多余出一个字节
					{
						HL_Mem_BLOCK new_hb;
						new_hb.m_begin=hb->m_begin+size;//设置块起点
						new_hb.m_end=hb->m_end;//设置块终点
						new_hb.Size=hb->Size-size;//设置块大小
						new_hb.Used=0;//标志未使用，可供下次分配
						HL_Mem_StoreBLOCK(new_hb,hp);
					}
					
					return (uint32_t*)hb->m_begin;
				}
			}
			if(HL_Mem_IsOverflow(hp->free_memptr,size,hp))//新分配一个块
			{
				HL_Mem_BLOCK hb;
				hb.m_begin=hp->free_memptr;//设置块起点
				hb.m_end=hp->free_memptr+size-1;//设置块终点
				hb.Size=size;//设置块大小
				hb.Used=1;//标志使用
				HL_Mem_StoreBLOCK(hb,hp);
				hp->free_memptr+=size;//内存池自由内存+=此次分配大小
				return (uint32_t*)hb.m_begin;//返回起始地址
			}
		}
	}
	return NULL;
}

void HL_Mem_Free(uint32_t m_begin,HL_Mem_Pool* hp)
{
	for(uint32_t i=0;i<hp->block_len;i++)
	{
		HL_Mem_BLOCK* hb=(HL_Mem_BLOCK*)(hp->pool+i*sizeof(HL_Mem_BLOCK));
		if(hb->m_begin==m_begin)
		{
			hb->Used=0;
			return;
		}
	}
}

uint8_t HL_Mem_IsOverflow(uint32_t mem_begin,uint32_t size,HL_Mem_Pool* hp)
{
	if(mem_begin+size-1>hp->free_memptr+hp->size-1) return 0;
	else return 1;
}

