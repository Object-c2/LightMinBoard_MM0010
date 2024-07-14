#include "AllFile.h"


#define UART1_RX_BUFF_SIZE     (u8)(50)
#define UART1_TX_BUFF_SIZE     (u8)(50)


u8 U8Uart1BakRxBuf[UART1_RX_BUFF_SIZE];
u8 U8Uart1RxBuf[UART1_RX_BUFF_SIZE];
u8 U8Uart1TxBuf[UART1_TX_BUFF_SIZE];
volatile u8 fGlobal_Uart1RxFlag = 0,U8Uart1TxHead = 0,U8SendDataLength = 0,U8Global_DoubleMode = 0;
volatile u8 fGlobal_N_Gear = 0,fGlobal_F_or_B_Mode = 0,fGlobal_WalkMode = 0,fGlobal_SpeedMode = 0;
volatile u8 fGlobal_Break = 0;
volatile u16 U16Global_AdcPower = 0;

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    UART1_IRQHandler
  * @brief   UART1串口中断
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

void UART1_IRQHandler(void)
{
	u8 U8RxData = 0,U8Tmp = 0,U8TxData = 0;
	static u8 U8RxNum = 0,U8TotalRxNum = 7;
	//发送数据
    if ((UART1->ISR & UART_IT_TXIEN) != 0)     //发送数据寄存器为空
	{
        //当前发送的数据，不等于当前数据帧的结尾就继续发送
        if (U8Uart1TxHead != U8SendDataLength)
        {
            U8TxData = U8Uart1TxBuf[U8Uart1TxHead++];
			UART_SendData(UART1,U8TxData);
        }
        //发送超过边界，就不再发送
        if (U8Uart1TxHead >= U8SendDataLength)
        {
			 //关闭发送寄存器空中断
            UART1->IER &= ~(1 << 0);
            U8Uart1TxHead = 0;
            U8SendDataLength = 0;
        }
		//清除发送寄存器空中断标志
        UART1->ICR = UART_IT_TXIEN;
    }
	//接收数据
    if ((UART1->ISR & UART_IT_RXIEN) != RESET)     //接收到数据
	{
        U8RxData = UART_ReceiveData(UART1);                //读取接收到的数据
		//清除中断标志位
        UART1->ICR = UART_IT_RXIEN;
        if (U8RxNum == 0)
        {
            //如果接收数据次数计数器刚好为0，并且数据刚好也为帧头，就累加数据，否则一直等待帧头
            if (U8RxData == 0X01)
            {
                U8Uart1RxBuf[U8RxNum++] = U8RxData;
                U8TotalRxNum = 4; //赋值一个最大值，防止在下面else if(U8RxNum>=U8TotalRxNum)这条语句出现逻辑错误
            }
            else if (U8RxData == 0X5A)
            {
                U8Uart1RxBuf[U8RxNum++] = U8RxData;
                U8TotalRxNum = 7; //赋值一个最大值，防止在下面else if(U8RxNum>=U8TotalRxNum)这条语句出现逻辑错误
            }
        }
        else if (U8RxNum < UART1_RX_BUFF_SIZE) //收到的数据字节数不能超过数组的边界
        {
            U8Uart1RxBuf[U8RxNum++] = U8RxData;
			if (U8RxNum == 5)
			{
				//在此处得到数据长度
				U8TotalRxNum = U8Uart1RxBuf[4] + 7;
				if (U8TotalRxNum > 149)
				{
					U8TotalRxNum = 149;
				}
			}
			else if (U8RxNum >= U8TotalRxNum)
			{
				if (fGlobal_Uart1RxFlag == 0)
				{
					//如果数据长度匹配，拷贝数据，本次接收结束，重新开始接受数据
					for (U8Tmp = 0; U8Tmp < U8TotalRxNum; U8Tmp++)
					{
						U8Uart1BakRxBuf[U8Tmp] = U8Uart1RxBuf[U8Tmp];
					}
					fGlobal_Uart1RxFlag = 1; //接收数据成功，标志位置一，然后在main中调用处理函数
				}
				//不管数据帧接收完成标志位是否成立fGlobal_Uart1RxFlag，下面的标志位都要清零
				U8RxNum = 0;
			}
            
        }
        else
        {
            U8RxNum = 0;    //超出接收缓存的长度，就不能接受
        }

    }
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    Crc16Calc
  * @brief   CRC16校验
  * @param   data_arr: 数组
**			 data_len: 数据长度
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

u16 Crc16Calc(u8 *data_arr, u8 data_len)
{
	u16 crc16 = 0;
	u8 i = 0;
	for(i = 0; i < data_len; i++)
	{
		crc16 = (uint16_t)(( crc16 >> 8) | (crc16 << 8));
		crc16 ^= data_arr[i];
		crc16 ^= (uint16_t)(( crc16 & 0xFF) >> 4);
		crc16 ^= (uint16_t)(( crc16 << 8) << 4);
		crc16 ^= (uint16_t)((( crc16 & 0xFF) << 4) << 1);
	}
	return crc16;
}


void Uart1SendData(u8 Data[], u8 len)
{
    u8 U8Tmp = 0, U8Tail;
     //等待发送寄存器空
    while (((UART1->CSR & (1 << 3)) == 0));
    for (U8Tmp = 0; U8Tmp < len; U8Tmp++)
    {
        U8Uart1TxBuf[U8SendDataLength++] = Data[U8Tmp];
    }
	UART1->TDR = U8Uart1TxBuf[U8Uart1TxHead++];
	
	//打开发送寄存器空中断
    UART1->IER |= (1 << 0);

}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    Uart1ReceiveData
  * @brief   串口数据接收函数
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
void Uart1ReceiveData(void)
{
	u8 U8Tmp = 0,U8Num = 0,*U8BakRxBuf,fSendEn = 0;
	u16 U16Tmp=0,U16Crc = 0,U16Cmd =0;
	u8 U8TxBuf[10];

	U8BakRxBuf = U8Uart1BakRxBuf;
	
	for(U8Tmp = 0;U8Tmp<10;U8Tmp++)
		U8TxBuf[U8Tmp] = 0;
	
	U8Num = U8BakRxBuf[4] + 5;
	U16Tmp = U8BakRxBuf[U8Num] * 256 + U8BakRxBuf[U8Num + 1];
	U16Crc = Crc16Calc(U8BakRxBuf, U8Num);
	U16Cmd = U8BakRxBuf[3]*256+U8BakRxBuf[2];                  //得到16位功能码
	U16Cmd&=0x00ff;  //兼容雅迪的协议  
	if((U16Tmp != U16Crc)||(U8Num>135))
		return;
	U8TxBuf[0]=0x5a;
	U8TxBuf[1]=0xb1;
	U8TxBuf[2]=U16Cmd&0xff;
	U8TxBuf[3]=(U16Cmd>>8)&0xff;
	switch(U16Cmd)
	{
		case 0x01:
			U8TxBuf[4] = 5;
			U8TxBuf[5] |= (fGlobal_Startup&0x01)<<7;
			fSendEn=1;                  //使能发送
		break;
    
    case 0x04://使用涛涛协议0x04(bit6改为喇叭，bit7改为尾灯，bit1改为行人模式标志位(待增加))
      U8BakRxBuf[4] = 2;
      if(U8BakRxBuf[6]&0x80)//有效不屏蔽
        fGlobal_TailLight = (U8BakRxBuf[5]&0x80)>>7;//尾灯
      if(U8BakRxBuf[6]&0x40)
        fGlobal_Horn = (U8BakRxBuf[5]&0x40)>>6;//喇叭
      if(U8BakRxBuf[6]&0x20)
        fGlobal_HeadLight = (U8BakRxBuf[5]&0x20)>>5;//前灯爆闪灯
      if(U8BakRxBuf[6]&0x10)
        fGlobal_BreakLight = (U8BakRxBuf[5]&0x10)>>4;//刹车灯
      //氛围灯为控制器控制
      if(U8BakRxBuf[6]&0x04)
        fGlobal_RightTurnLight = (U8BakRxBuf[5]&0x04)>>2;//右转向灯
      if(U8BakRxBuf[6]&0x02)
        fGlobal_LeftTurnLight = (U8BakRxBuf[5]&0x02)>>1;//左转向灯
      if(U8BakRxBuf[6]&0x01)
        fGlobal_WalkFlag = (U8BakRxBuf[5]&0x01);//行人档
      fSendEn=1;                  //使能发送
      break;
		default:
      break;	
	}
	if(fSendEn)
	{
    switch(U16Cmd)
    {
      case 0x01:
        U8Num=U8TxBuf[4]+5;
        U16Crc = Crc16Calc(U8TxBuf,U8Num);
        U8TxBuf[U8Num++] = U16Crc>>8;
        U8TxBuf[U8Num++] = U16Crc&0xff;
        Uart1SendData(U8TxBuf,U8Num);
        break;
      case 0x04:
        U8Num=U8TxBuf[4]+5;
        for(u8 i = 0;i < 2;i++)
        {
          U8TxBuf[5+i] = U8BakRxBuf[5+i];
        }
        U16Crc = Crc16Calc(U8TxBuf,U8Num);
        U8TxBuf[U8Num++] = U16Crc>>8;
        U8TxBuf[U8Num++] = U16Crc&0xff;
        Uart1SendData(U8TxBuf,U8Num);
        break;
      default:
        break;
    }
		
	}
}

