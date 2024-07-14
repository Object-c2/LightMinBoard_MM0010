/**
  ************************************* Copyright ****************************** 
  *
  *                 (C) Copyright 2023,楠瘦,China, GCU.
  *                            All Rights Reserved
  *                              
  *                     By(nanshou)
  *                     
  *    
  * FileName   : init.c   
  * Version    : v1.0		
  * Author     : 			
  * Date       : 2023-03-13         
  * Description:    
  * Function List:  
  	1. ....
  	   <version>: 		
  <modify staff>:
  		  <data>:
   <description>:  
  	2. ...
  ******************************************************************************
 */
#include "AllFile.h"

#define DEBUG
UWs2812Color Ws2812Color[WSNUM] = {0};
UWs2812Brightness Ws2812Brightness[WSBRIGHTNESSDIF] = {
  {.U8Ws2812Brightness[0] = 255,.U8Ws2812Brightness[1] = 255,.U8Ws2812Brightness[2] = 255,},
  {.U8Ws2812Brightness[0] = 255,.U8Ws2812Brightness[1] = 255,.U8Ws2812Brightness[2] = 255,},
  {.U8Ws2812Brightness[0] = 255,.U8Ws2812Brightness[1] = 255,.U8Ws2812Brightness[2] = 255,},
};
//uint8_t U8SendWs2812Data[WSNUM][24] = {0};
uint8_t U8SendWs2812Data[WSNUM][27] = {0};

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    gpio_init
  * @brief   GPIO初始化
  * @param   None
  * @retval
  * @author 
  * @Data    2023-03-13
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void gpio_init(void)
{
	/*
		PA0     LS_CTRL
		PA1     SPI  
		PA2     LKS_C
		PA3     RXD
		PA4     HD_CTRL
		PA5     STK_C
		PA6     LEFT_LED1
		PA7     LEFT_LED2
		PA8     RIGHT_LED1
		PA9     RIGHT_LED2
		PA10    LK_C
		PA11    HDK_C
		PA12    TXD
		PA13    SWDIO
		PA14    SWCLK
		PA15    RK_C	
	*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);
	//上拉输入
	GPIO_InitStructure.GPIO_Pin=/*GPIO_Pin_2|*/GPIO_Pin_5|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_15;  //
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;  
	GPIO_Init(GPIOA,&GPIO_InitStructure);		
	//推挽输出                   喇叭          爆闪灯      前灯        尾灯       刹车灯     转向灯1     转向灯2
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|/*GPIO_Pin_6|*/GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
  
	//复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6/*|GPIO_Pin_12*/;//PA1原本是在这里初始化的
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;  
	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	//浮空输入
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3; 
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_FLOATING;  
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);  //复用为UART1_RX	
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_1); //复用为UART1_TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);  //复用为UART1_RX	


	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                           //Output speed
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                               //GPIO mode
	GPIO_Init(GPIOB, &GPIO_InitStructure);
//  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0,GPIO_AF_0);

//  GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                           //Output speed
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                               //GPIO mode
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    uart_init
  * @brief   串口初始化
  * @param   None
  * @retval
  * @author  
  * @Data    2023-03-13
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void uart1_init(u32 bound)
{
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART1, ENABLE);
	UART_InitTypeDef UART_InitStructure;
	UART_InitStructure.BaudRate = bound;
    UART_InitStructure.WordLength = UART_WordLength_8b;
    UART_InitStructure.StopBits = UART_StopBits_1;
    UART_InitStructure.Parity = UART_Parity_No;
    UART_InitStructure.Mode = UART_Mode_Rx|UART_Mode_Tx;
    UART_InitStructure.HWFlowControl = UART_HWFlowControl_None;
    UART_Init(UART1, &UART_InitStructure);	
	//清除中断标志位
    UART1->ICR = UART_IT_RXIEN;
    //使能接受中断
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);
	UART_Cmd(UART1, ENABLE);		
}


void spi1_init(void)
{
	SPI_InitTypeDef SPI1_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_SPI1, ENABLE);
	//
//	SPI1_InitStructure.SPI_BaudRatePrescaler = ;
//	SPI1_InitStructure.SPI_CPHA = ;
//	SPI1_InitStructure.SPI_CPOL = ;
//	SPI1_InitStructure.SPI_DataSize = ;
//	SPI1_InitStructure.SPI_DataWidth = ;
//	SPI1_InitStructure.SPI_FirstBit = ;
//	SPI1_InitStructure.SPI_Mode = ;
//	SPI1_InitStructure.SPI_NSS = ;
//	SPI_StructInit(&SPI1_InitStructure);
//	SPI_Init(SPI1, &SPI1_InitStructure);
//	
//	SPI_Cmd(SPI_TypeDef* spi, FunctionalState state);
}


/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    timer_init
  * @brief   定时器初始化
  * @param   None
  * @retval
  * @author  
  * @Data    2023-03-13
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void timer_init(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//TIM1
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM1, ENABLE); 
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 3072-1;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    //Setting Clock Segmentation
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//0.00000002
    //TIM Upward Counting Mode
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	
	
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM3, ENABLE); //48M
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 7-1;//4-1;//140ns进入一次中断
    TIM_TimeBaseStructure.TIM_Prescaler = 1-1;//48M
    //Setting Clock Segmentation
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    //TIM Upward Counting Mode
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

static void ADCxSampleTimeConfig(ADC_TypeDef* ADCn, ADCSAM_TypeDef sampleTime)
{
    ADCn->CFGR &= ~ADC_CFGR_SAMCTL;
    ADCn->CFGR |= sampleTime;
}

void adc1_init(void)
{
	ADC_InitTypeDef ADC1_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_ADC1, ENABLE);                                         //Enable ADC clock
	ADC1_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC1_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC1_InitStructure.ADC_ExternalTrigConv = ADC1_ExternalTrigConv_T1_TRIG;
	ADC1_InitStructure.ADC_Mode = ADC_Mode_Imm;
	ADC1_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;
	ADC1_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_Init(ADC1, &ADC1_InitStructure);
	ADCxSampleTimeConfig(ADC1, ADC_Samctl_28_5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 0, ADC_Samctl_28_5);//ADCSAM_TypeDef
	ADC_Cmd(ADC1, ENABLE);
	ADC_ANY_Cmd(ADC1, DISABLE);                                                 //Enable and use ANYChan to control ADC sample
	ADC_ANY_NUM_Config(ADC1, 0);//此寄存器是配置使用几个通道，0为使用1个通道0
	ADC_ANY_CH_Config(ADC1, 0, ADC_Channel_1);//CHANY_SEL0为模拟通道0 //ADC_ANY_CH_Config(ADC1, 0, ADC_Channel_5);//CHANY_SEL0为模拟通道0//                                  //assign ADC channel 1 to RANK 0;
	ADC_ANY_Cmd(ADC1, ENABLE);                                                  //Enable and use ANYChan to control ADC sample
//	ADC_ExternalTrigConvCmd(ADC1, ENABLE);
}


void nvic_init(void)
{
	exNVIC_Init_TypeDef  NVIC_InitStruct;
//	NVIC_InitStruct.NVIC_IRQChannel = UART1_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
//	exNVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_BRK_UP_TRG_COM_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	exNVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	exNVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM1, ENABLE);
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    hardware_init
  * @brief   硬件初始化
  * @param   None
  * @retval
  * @author  
  * @Data    2023-03-13
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
void hardware_init(void)
{
//  RCC->CR &= 0xFFFAFFFF;
//  RCC->SYSCFGR &= 0xBFFF;
	gpio_init();               //GPIO初始化
//	uart1_init(38400);          //串口初始化
	spi1_init();
	timer_init();
	adc1_init();
	nvic_init();               //中断使能
}


/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    Software_init
  * @brief   软件初始化
  * @param   None
  * @retval
  * @author  
  * @Data    2023-03-13
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void Software_init(void)
{
	static u8 U8_200msCnt = 0;
	//延时一段时间等待单片机稳定
	for(U8_200msCnt = 0;U8_200msCnt<200;U8_200msCnt++)
	{
		while(fGlobal_vu8ReadyFlag == 1 && fGlobal_1ms == 0){};
		fGlobal_1ms = 0;
	}
}
