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
		if(HL_Mem_IsOverflow(hp->free_memptr,size,hp))//û�з�����ڴ棬���Ҵ�С�������
		{
			HL_Mem_BLOCK hb;
			hb.m_begin=hp->free_memptr;//���ÿ����
			hb.m_end=hp->free_memptr+size-1;//���ÿ��յ�
			hb.Size=size;//���ÿ��С
			hb.Used=1;//��־ʹ��
			
			HL_Mem_StoreBLOCK(hb,hp);
			hp->free_memptr+=size;//�ڴ�������ڴ�+=�˴η����С
			return (uint32_t*)hb.m_begin;//������ʼ��ַ
		}
	}
	else if(hp->block_len>0)
	{
		//ע��������+2����Ϊ����������ĳ����л����������
		if(hp->block_len+2>hp->managePool_size) return NULL;//��������˾��޷�����
		else
		{
			for(uint32_t i=0;i<hp->block_len;i++)
			{
				HL_Mem_BLOCK* hb=(HL_Mem_BLOCK*)(hp->pool+i*sizeof(HL_Mem_BLOCK));
				if(hb->Used==0&&hb->Size>=size)//Ѱ�ұ������Ҵ�С�ʺϵĿ�
				{
					hb->Used=1;//��λ
					//�ж��Ƿ��ж�������ڴ棬���䴴��Ϊһ���¿�
					if(hb->Size-size>1)//���ٶ����һ���ֽ�
					{
						HL_Mem_BLOCK new_hb;
						new_hb.m_begin=hb->m_begin+size;//���ÿ����
						new_hb.m_end=hb->m_end;//���ÿ��յ�
						new_hb.Size=hb->Size-size;//���ÿ��С
						new_hb.Used=0;//��־δʹ�ã��ɹ��´η���
						HL_Mem_StoreBLOCK(new_hb,hp);
					}
					
					return (uint32_t*)hb->m_begin;
				}
			}
			if(HL_Mem_IsOverflow(hp->free_memptr,size,hp))//�·���һ����
			{
				HL_Mem_BLOCK hb;
				hb.m_begin=hp->free_memptr;//���ÿ����
				hb.m_end=hp->free_memptr+size-1;//���ÿ��յ�
				hb.Size=size;//���ÿ��С
				hb.Used=1;//��־ʹ��
				HL_Mem_StoreBLOCK(hb,hp);
				hp->free_memptr+=size;//�ڴ�������ڴ�+=�˴η����С
				return (uint32_t*)hb.m_begin;//������ʼ��ַ
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

