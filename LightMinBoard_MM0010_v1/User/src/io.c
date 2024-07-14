#include "AllFile.h"

//踩刹车尾灯+氛围灯常量
//前灯信号打开爆闪灯+尾灯
//DL1尾灯DL2刹车灯
//
volatile u8 fGlobal_Startup = 0;
volatile u8 fGlobal_LeftTurnLight = 0,fGlobal_RightTurnLight = 0,fGlobal_Horn = 0,fGlobal_HeadLight = 0;
volatile u8 fGlobal_TailLight = 0,fGlobal_BreakLight = 0,fGlobal_WalkFlag = 0, U8Global_Send = 0;
#define UARTHORN 0
#define KEYCONTROL 1 

void WS2812_DigitalConvers(void);
/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    IO_handle
  * @brief   灯光处理
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
volatile u8 fGlobal_LeftLed =0,fGlobal_RightLed = 0,fGloal_Beer =0,fGloal_KeyStatus = 0;
void IO_handle(void)
{
	WS2812_DigitalConvers();
}

void WS2812_DigitalConvers(void)
{	
  for(u8 i = 0;i<WSNUM;i++)//灯珠数量
  {
		
#if WSSPI8BIT == 1
    for(u8 j = 0;j<=3;j++)//三种颜色//0XC0=0 0XFE=1
    {
      u8 bitsnum = 0x80;
      for(u8 k = 0;k<8;k++)
      {
        if((Ws2812Color[i].U8Ws2812Color[j]&bitsnum) == bitsnum)
        {
          U8SendWs2812Data[i][j*8+k] = 0xFC;//0xFE;//写1
        }
        else
        {
          U8SendWs2812Data[i][j*8+k] = 0xc0;//写0
        }
        bitsnum >>= 1;
      }
    }
#endif
  }
	
}



