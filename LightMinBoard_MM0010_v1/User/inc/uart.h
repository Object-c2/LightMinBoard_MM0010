#ifndef __UART_H_
#define __UART_H_

#include <types.h>

extern volatile u8 fGlobal_Uart1RxFlag,fGlobal_Break,U8Global_DoubleMode,fGlobal_WalkMode;
extern volatile u8 fGlobal_SpeedMode,fGlobal_LeftLed,fGlobal_RightLed;
extern volatile u16 U16Global_AdcPower;

extern void Uart1ReceiveData(void);
extern u16 Crc16Calc(u8 *data_arr, u8 data_len);


#endif
