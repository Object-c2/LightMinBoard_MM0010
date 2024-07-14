#ifndef __IO_H_
#define __IO_H_




#define Beer_IN         (GPIOA->IDR>>2&0x01)
#define F_IN            (GPIOA->IDR>>5&0x01)
#define T_IN            (GPIOA->IDR>>10&0x01)
#define FT_IN            (GPIOA->IDR>>15&0x01)
#define SecondStartup_IN  (GPIOA->IDR>>11&0x01)


#define Beer_ON     (GPIOA->ODR |= (1<<0))//(GPIOA->ODR |= (1<<4))
#define Beer_OFF    (GPIOA->ODR &= ~(1<<0))//(GPIOA->ODR &= ~(1<<4))

#define HeadLight_ON    (GPIOA->ODR |= (1<<4))
#define HeadLight_OFF   (GPIOA->ODR &= ~(1<<4))

#define TailLight_ON    (GPIOA->ODR |= (1<<6))
#define TailLight_OFF   (GPIOA->ODR &= ~(1<<6))

#define BreakLight_ON   (GPIOA->ODR |= (1<<7))
#define BreakLight_OFF  (GPIOA->ODR &= ~(1<<7))

#define RightLight_ON   (GPIOA->ODR |= (1<<8))
#define RightLight_OFF  (GPIOA->ODR &= ~(1<<8))

#define LeftLight_ON    (GPIOA->ODR |= (1<<9))
#define LeftLight_OFF   (GPIOA->ODR &= ~(1<<9))


//////////////////////////////////////WS2812/////////////////////////////////////
#define WSNUM       3  //控制的ws2812灯珠数量
#define WSSPI8BIT   1  //SPI8位模式
#define WSSPI16BIT  0
#define WSBRIGHTNESSDIF      3  //相邻的三个灯珠亮度不同
/////////////////////////////////////////////////////////////////////////////////

//端口宏定义在bsp_SysInit.h文件中定义
#if WSSPI8BIT == 1
//extern uint8_t U8SendWs2812Data[WSNUM][24];//这个＋1为结束码
extern uint8_t U8SendWs2812Data[WSNUM][27];//这个＋1为结束码
#endif
typedef struct WS2812{
  u8 GREEN;
  u8 RED;
  u8 BLUE;
}SWs2812ColorAndBrightness;

typedef union
{
  SWs2812ColorAndBrightness Color;
  u8 U8Ws2812Color[3];
}UWs2812Color;
extern UWs2812Color Ws2812Color[WSNUM];

typedef union
{
  SWs2812ColorAndBrightness Brightness;
  u8 U8Ws2812Brightness[3];
}UWs2812Brightness;//
extern UWs2812Brightness Ws2812Brightness[WSBRIGHTNESSDIF];


extern volatile u8 fGlobal_Startup;
extern volatile u8 fGlobal_LeftTurnLight,fGlobal_RightTurnLight,fGlobal_Horn,fGlobal_HeadLight;
extern volatile u8 fGlobal_TailLight,fGlobal_BreakLight,fGlobal_WalkFlag,U8Global_Send;

extern void IO_handle(void);

#endif
