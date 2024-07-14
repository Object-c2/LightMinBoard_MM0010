#ifndef __UART2_H_
#define __UART2_H_


extern volatile u8 fGlobal_Uart2NoReceviceErr,fGlobal_handleBarErr,fGlobal_EbsErr,fGlobal_HallorAngleErr;
extern volatile u8 fGlobal_MuchCurrent,fGlobal_StaticCurrentErr,fGlobal_BatteryErr,fGlobal_BatteryOverlowLim;
extern volatile u8 U8Global_F_or_B_Mode,fGlobalBrakePort;

extern void Uart2_SendData(void);

#endif



 