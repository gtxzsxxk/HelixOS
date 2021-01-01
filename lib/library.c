#include "library.h"

void HL_Lib_ARRAY_LEFTMOVE(uint8_t* buffer,uint8_t* tempbuffer,uint8_t len)
{
	//00001234
	//12340000
	memset(tempbuffer,0,sizeof(tempbuffer));
	for(uint8_t i=len;i<strlen((const char*)buffer);i++)
	{
		tempbuffer[i-len]=buffer[i];
	}
	memcpy(buffer,tempbuffer,HL_TERMINAL_BUFFER_SIZE);
}

uint8_t HL_RegexString(uint8_t* beDetected,uint8_t* key)
{
	uint8_t i=0;
	uint8_t j;//key¦Ì???¡À¨º 
	uint8_t b_temp=0;
	uint8_t bd_len=strlen((const char*)beDetected);
	uint8_t key_len=strlen((const char*)key);
	if(key_len>bd_len) return 0;
	for(i=0;i<bd_len;i++)
	{
		if(key[0]==beDetected[i])
		{
			if(key_len+i>bd_len) break;//¨¨?1? 3?3?¡¤??¡ì 
			b_temp=i+1;//beDetected¦Ì???¡À¨º 
			for(j=1;j<key_len;j++)//¡À¨¦¨¤¨²key?D¦Ì?¡Á?¡¤? 
			{
				if(key[j]!=beDetected[b_temp++]) break;
				if(j==key_len-1)//¨¨?1??-?¡¤?¨¢¨º?
				return b_temp-1; 
			}
		}
	}
	return 0;
}

uint8_t HL_CompareString(const char* a,const char* b)
{
	if(strlen(a)!=strlen(b))
		return 0;
	uint16_t len=strlen(a),cnt=0;
	for(uint16_t i=0;i<len;i++)
	{
		if(a[i]==b[i]) cnt++;
	}
	if(cnt!=len) return 0;
	else return 1;
}

void HL_ScanNext(uint8_t* from ,uint8_t* buffer)
{
	//01234567890
	//command abcdef
	uint8_t i;
	for(i=0;i<strlen((const char*)from);i++)
	{
		if(from[i]==' ')
		{
			break;
		}
	}
	memcpy(buffer,from,i);
}

void HL_CPU_USAGE_ADD(HL_Thread* ht)
{
	ht->Ticks++;
}

