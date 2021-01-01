#include "app_musicPlayer.h"

void HAPP_musicPlayer_Handler(void)
{
	uint8_t soundname[64];
	memset(soundname,0,sizeof(soundname));
	HL_Terminal_Write("Enter the path:\r\n");
	HL_Terminal_ReadLine(soundname);
	
	for(uint8_t i=0;i<strlen((const char*)soundname);i++)
	{
		if(soundname[i]=='.')
		{
			if((soundname[i+1]=='d'&&
				 soundname[i+2]=='l'&&
				 soundname[i+3]=='s'&&
				 soundname[i+4]=='s')||
				(soundname[i+1]=='D'&&
				 soundname[i+2]=='L'&&
				 soundname[i+3]=='S'&&
				 soundname[i+4]=='S'))
			{
				HAPP_musicPlayer_playDLSS(soundname);
			}
			else if((soundname[i+1]=='w'&&
				 soundname[i+2]=='a'&&
				 soundname[i+3]=='v')||
				(soundname[i+1]=='W'&&
				 soundname[i+2]=='A'&&
				 soundname[i+3]=='V'))
			{
				HAPP_musicPlayer_playWAV(soundname);
			}
		}
	}
	
	hfree(soundname);
}

void Delay()
{
	for(uint32_t i=0;i<275;i++);
}

void HAPP_musicPlayer_playDLSS(uint8_t* path)
{
	
	uint32_t br;
	uint8_t mbuffer[512];
	memset(mbuffer,0,512);
	HL_SCHEDULER_STOP();
	retSD=f_open(&SDFile,(const char*)path,FA_OPEN_EXISTING|FA_READ);
	/*TIM3->CNT=0;
	TIM3->PSC=8-1;
	TIM3->ARR=10-1;
	TIM3->CR1=0x0004;
	TIM3->CR1|=1<<2;
	TIM3->DIER|=1;
	TIM3->DIER|=1<<6;*/
	TIM3->PSC=5-1;
	TIM3->ARR=653-1;
	while(!f_eof(&SDFile))
	{
		/*TIM3->CNT=0;
		TIM3->SR=0;
		TIM3->CR1|=1;//开定时器*/
		HL_SCHEDULER_STOP();
		retSD=f_read(&SDFile,mbuffer,512,&br);
		f_lseek(&SDFile,f_tell(&SDFile)+br);
		HL_SCHEDULER_START();
		
		for(uint32_t i=0;i<br;i++)
		{
			
			
			HL_DAC_UpdateValue(mbuffer[i]);
			HAL_TIM_Base_Start(&htim3);
			htim3.Instance->SR&= 0xfffffffe;//清溢出标志
			while((htim3.Instance->SR & 0x01) == 0x00);//等待溢出标志
      HAL_TIM_Base_Stop(&htim3);// 停止定时器
			//Delay();
			/*TIM3->CNT=0;
			TIM3->SR=0;
			TIM3->CR1|=1;//开定时器
			while(!(TIM3->SR&0x01));
			TIM3->SR&=~1;
			TIM3->CR1&=~1;*/
		}
	}
	
	f_close(&SDFile);
}

void HAPP_musicPlayer_playWAV(uint8_t* path)
{
	uint8_t head_buffer[44];
	uint8_t mbuffer[512];
	uint32_t br;
	WAV_RIFF wr;
	WAV_FMT wf;
	WAV_data wd;
	
	memset(head_buffer,0,sizeof(head_buffer));
	HL_SCHEDULER_STOP();//NOT ALLOWED TO INTERRUPT
	retSD=f_open(&SDFile,(const char*)path,FA_OPEN_ALWAYS|FA_READ);
	retSD=f_read(&SDFile,head_buffer,44,&br);
	f_lseek(&SDFile,44);
	HL_SCHEDULER_START();
	
	memcpy(&wr,head_buffer,12);
	memcpy(&wf,head_buffer+12,24);
	memcpy(&wd,head_buffer+36,8);
	
	//print out the file information
	uint8_t * tempbuf=hmalloc(256);
	memset(tempbuf,0,256);
	sprintf((char*)tempbuf,"[WAV FILE]\r\nNumChannels=%u\r\nSampleRate=%u\r\nByteRate=%u\r\nBitsPerSample=%u",
	wf.NumChannels,wf.SampleRate,wf.ByteRate,wf.BitsPerSample);
	HL_Terminal_Write(tempbuf);
	hfree(tempbuf);
	
	if(wf.SampleRate==11025)
	{
		TIM3->PSC=5-1;
		TIM3->ARR=1306-1;
	}
	else if(wf.SampleRate==22050)
	{
		TIM3->PSC=5-1;
		TIM3->ARR=653-1;
	}
	else if(wf.SampleRate==44100)
	{
		TIM3->PSC=8-1;
		TIM3->ARR=204-1;
	}
	else if(wf.SampleRate==8000)
	{
		TIM3->PSC=5-1;
		TIM3->ARR=1800-1;
	}
	
	while(!f_eof(&SDFile))
	{
		HL_SCHEDULER_STOP();
		retSD=f_read(&SDFile,mbuffer,512,&br);
		f_lseek(&SDFile,f_tell(&SDFile)+br);
		HL_SCHEDULER_START();
		
		//begin to play the music
		if(wf.NumChannels==1)
		{
			if(wf.BitsPerSample==8)
			{
				for(uint8_t i=0;i<br;i++)
				{
					HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_8B_R,mbuffer[i]);
					HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
					
					HAL_TIM_Base_Start(&htim3);
					htim3.Instance->SR&= 0xfffffffe;//清溢出标志
					while((htim3.Instance->SR & 0x01) == 0x00);//等待溢出标志
					HAL_TIM_Base_Stop(&htim3);// 停止定时器
				}
			}
			else if(wf.BitsPerSample==16)
			{
				for(uint8_t i=0;i<br;i+=2)
				{
					uint8_t lowbit=mbuffer[i];
					uint8_t highbit=mbuffer[i+1];
					uint16_t sdata=(((uint8_t)(highbit-0x80)<<4)|(lowbit>>4));
					
					HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,sdata);
					HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
					
					HAL_TIM_Base_Start(&htim3);
					htim3.Instance->SR&= 0xfffffffe;//清溢出标志
					while((htim3.Instance->SR & 0x01) == 0x00);//等待溢出标志
					HAL_TIM_Base_Stop(&htim3);// 停止定时器
				}
			}
		}
		else if(wf.NumChannels==2)
		{
			if(wf.BitsPerSample==8)
			{
				for(uint8_t i=0;i<br;i+=2)
				{
					HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_8B_R,mbuffer[i]);
					HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
					HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_8B_R,mbuffer[i+1]);
					HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
					
					HAL_TIM_Base_Start(&htim3);
					htim3.Instance->SR&= 0xfffffffe;//清溢出标志
					while((htim3.Instance->SR & 0x01) == 0x00);//等待溢出标志
					HAL_TIM_Base_Stop(&htim3);// 停止定时器
				}
			}
			else if(wf.BitsPerSample==16)
			{
				for(uint8_t i=0;i<br;i+=4)
				{
					HAL_TIM_Base_Start(&htim3);
					htim3.Instance->SR&= 0xfffffffe;//清溢出标志
					
					uint8_t lowbit=mbuffer[i];
					uint8_t highbit=mbuffer[i+1];
					uint16_t sdata=(((uint8_t)(highbit-0x80)<<4)|(lowbit>>4));
					
					HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,sdata);
					HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
					
					lowbit=mbuffer[i+2];
					highbit=mbuffer[i+3];
					sdata=(((uint8_t)(highbit-0x80)<<4)|(lowbit>>4));
					
					HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,sdata);
					HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
					
					
					while((htim3.Instance->SR & 0x01) == 0x00);//等待溢出标志
					HAL_TIM_Base_Stop(&htim3);// 停止定时器
				}
			}
		}
	}
	
}
