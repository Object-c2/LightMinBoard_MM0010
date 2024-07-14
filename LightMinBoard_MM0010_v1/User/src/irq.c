#include "AllFile.h"

volatile u8 fGlobal_1ms = 0;
volatile u16 fGlobal_vu16IlluminData[4] = {0};
volatile u8 fGlobal_vu8SampleOverTimeFlag = 0, fGlobal_vu8ReadyFlag = 0;

/*
u8samplecount 采样次数
*/
void ADC_Sample(u8 u8samplecount)
{
	static u8 i = 0;
	u16 u16ConvertErr = 0;
	
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(ADC_GetFlagStatus(ADC1, ADC_IT_EOC) == 0)
	{
		if(u16ConvertErr++ > 1000)
		{
			fGlobal_vu8SampleOverTimeFlag = 1;
			return;
		}
	}//死循环+计数退出ad采样判断
	if(fGlobal_vu8SampleOverTimeFlag == 0)
	{
		ADC_ClearFlag(ADC1, ADC_IT_EOC);
		if(i < u8samplecount)
			fGlobal_vu16IlluminData[i++] = ADC_GetConversionValue(ADC1);
		if(i == u8samplecount)
			i = 0;
	}
	if(fGlobal_vu8ReadyFlag == 0)
	{
		if(fGlobal_vu8SampleOverTimeFlag == 1 && i == 0)
			fGlobal_vu8ReadyFlag = 1;
	}
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void)     //64us进入一次
{
	static u8 U8_1msCnt = 0;
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		fGlobal_vu8SampleOverTimeFlag = 0;
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		
		ADC_Sample(4);
    
//		if(t++ == 2)//2*64us
//		{
//			t = 0;
//			
//			if(i <= WSNUM)
//			{
//				if(i == WSNUM)
//					i = 0;
//				if(j <= 4)
//				{
//					if(j == 4)
//						j = 0;
//					if(k <= 8)
//					{
//						if(k == 8)
//						{
//							k = 0;
//							U8Global_Send = 1;
//							bitsnum = 0x01;
////							bitsnum = 0x80;//????1
//						}
//						testtemp = (U8SendWs2812Data[i][j*8+k] & bitsnum);//0x80??0x08??????
//						if((U8SendWs2812Data[i][j*8+k] & bitsnum) == bitsnum)
//						{
//							GPIOA->ODR |= (1<<1);
//						}
//						else
//						{
//							GPIOA->ODR &= ~(1<<1);
//						}
//						bitsnum <<= 1;
////						bitsnum >>= 1;
//					}
//				}
//				k++;
//				j++;
//				i++;
//			}
//		}
		
			
		
		if(U8_1msCnt<16)
			U8_1msCnt++;
		else
		{
			fGlobal_1ms = 1;
			U8_1msCnt = 0;
		}
		
		
		
		
	}
}

void TIM3_IRQHandler(void)
{
	static u8 i = 0,j = 0,k = 0,bitsnum = /*??????0x01,*/0x80,t = 0;
	static u8 testtemp = 0;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		
		if(U8Global_Send == 1 && t++ == 1)//1*164ns
		{
			t = 0;
			
			if(U8Global_Send == 1 && i < WSNUM)
			{
				if(j < 27)//24)
				{
					if(k < 8)
					{
//						testtemp = (U8SendWs2812Data[i][j] & bitsnum);//0x80??0x08??????
						if((U8SendWs2812Data[i][j] & bitsnum) == bitsnum)
//						if(testtemp++ < 1)
						{
							GPIOA->ODR |= (1<<1);
						}
						else
						{
							GPIOA->ODR &= ~(1<<1);//???????
							testtemp = 0;
						}
//						bitsnum <<= 1;
						bitsnum >>= 1;
						
						k++;
						if(k == 8)
						{
							k = 0;//??????????
							j++;
//							bitsnum = 0x01;
							bitsnum = 0x80;//????1
						}
					}
					if(j == 27)//24)
					{
						j = 0;
						i++;
					}
				}
			}
			if(U8Global_Send == 1 && i == WSNUM)// && j == 3 && k == 8)
			{//24*WSNUM???????
				U8Global_Send = 0;
				i = 0;
				j = 0;
				k = 0;
				TIM_Cmd(TIM3, DISABLE);
			}
		}
		
	}
}

