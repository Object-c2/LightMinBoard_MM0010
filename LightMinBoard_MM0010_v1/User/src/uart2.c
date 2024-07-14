#include "AllFile.h"


#define UART2_RX_BUFF_SIZE     (u8)(50)
#define UART2_TX_BUFF_SIZE     (u8)(50)


u8 U8Uart2BakRxBuf[UART2_RX_BUFF_SIZE];
u8 U8Uart2RxBuf[UART2_RX_BUFF_SIZE];
u8 U8Uart2TxBuf[UART2_TX_BUFF_SIZE];
volatile u8 fGlobal_Uart2RxFlag = 0,U8Uart2TxHead = 0,U8UART2SendDataLength = 0,fGlobal_Uart2NoReceviceErr = 0;
volatile u8 fGlobal_handleBarErr = 0,fGlobal_EbsErr = 0,fGlobal_HallorAngleErr = 0,fGlobal_MuchCurrent = 0;
volatile u8 fGlobal_StaticCurrentErr = 0,fGlobal_BatteryOverlowLim = 0,fGlobal_BatteryErr = 0,U8Global_F_or_B_Mode = 0;
volatile u8 fGlobalBrakePort = 0;
/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    UART2_IRQHandler
  * @brief   UART2�����ж�
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

void UART2_IRQHandler(void)
{
	u8 U8RxData = 0,U8Tmp = 0,U8TxData = 0;
	static u8 U8RxNum = 0,U8TotalRxNum = 7;
	//��������
    if ((UART2->ISR & UART_IT_TXIEN) != 0)     //�������ݼĴ���Ϊ��
	{
        //��ǰ���͵����ݣ������ڵ�ǰ����֡�Ľ�β�ͼ�������
        if (U8Uart2TxHead != U8UART2SendDataLength)
        {
            U8TxData = U8Uart2TxBuf[U8Uart2TxHead++];
			UART_SendData(UART2,U8TxData);
        }
        //���ͳ����߽磬�Ͳ��ٷ���
        if (U8Uart2TxHead >= U8UART2SendDataLength)
        {
			 //�رշ��ͼĴ������ж�
            UART2->IER &= ~(1 << 0);
            U8Uart2TxHead = 0;
            U8UART2SendDataLength = 0;
        }
		//������ͼĴ������жϱ�־
        UART2->ICR = UART_IT_TXIEN;
    }
	//��������
    if ((UART2->ISR & UART_IT_RXIEN) != RESET)     //���յ�����
	{
        U8RxData = UART_ReceiveData(UART2);                //��ȡ���յ�������
		//����жϱ�־λ
        UART2->ICR = UART_IT_RXIEN;
        if (U8RxNum == 0)
        {
            //����������ݴ����������պ�Ϊ0���������ݸպ�ҲΪ֡ͷ�����ۼ����ݣ�����һֱ�ȴ�֡ͷ
            if (U8RxData == 0X01)
            {
                U8Uart2RxBuf[U8RxNum++] = U8RxData;
                U8TotalRxNum = 4; //��ֵһ�����ֵ����ֹ������else if(U8RxNum>=U8TotalRxNum)�����������߼�����
            }
            else if (U8RxData == 0X5A)
            {
                U8Uart2RxBuf[U8RxNum++] = U8RxData;
                U8TotalRxNum = 7; //��ֵһ�����ֵ����ֹ������else if(U8RxNum>=U8TotalRxNum)�����������߼�����
            }
        }
        else if (U8RxNum < UART2_RX_BUFF_SIZE) //�յ��������ֽ������ܳ�������ı߽�
        {
            U8Uart2RxBuf[U8RxNum++] = U8RxData;
			if (U8RxNum == 5)
			{
				//�ڴ˴��õ����ݳ���
				U8TotalRxNum = U8Uart2RxBuf[4] + 7;
				if (U8TotalRxNum > 149)
				{
					U8TotalRxNum = 149;
				}
			}
			else if (U8RxNum >= U8TotalRxNum)
			{
				if (fGlobal_Uart2RxFlag == 0)
				{
					//������ݳ���ƥ�䣬�������ݣ����ν��ս��������¿�ʼ��������
					for (U8Tmp = 0; U8Tmp < U8TotalRxNum; U8Tmp++)
					{
						U8Uart2BakRxBuf[U8Tmp] = U8Uart2RxBuf[U8Tmp];
					}
					fGlobal_Uart2RxFlag = 1; //�������ݳɹ�����־λ��һ��Ȼ����main�е��ô�����
				}
				//��������֡������ɱ�־λ�Ƿ����fGlobal_Uart2RxFlag������ı�־λ��Ҫ����
				U8RxNum = 0;
			}
            
        }
        else
        {
            U8RxNum = 0;    //�������ջ���ĳ��ȣ��Ͳ��ܽ���
        }

    }
}


void Uart2SendData(u8 Data[], u8 len)
{
    u8 U8Tmp = 0;
     //�ȴ����ͼĴ�����
    while (((UART2->CSR & (1 << 3)) == 0));
    for (U8Tmp = 0; U8Tmp < len; U8Tmp++)
    {
        U8Uart2TxBuf[U8UART2SendDataLength++] = Data[U8Tmp];
    }
	UART2->TDR = U8Uart2TxBuf[U8Uart2TxHead++];
	
	//�򿪷��ͼĴ������ж�
    UART2->IER |= (1 << 0);

}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    UART2ReceiveData
  * @brief   �������ݽ��պ���
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
void Uart2ReceiveData(void)
{
	u8 U8Num = 0,*U8BakRxBuf;
	u16 U16Tmp=0,U16Crc = 0,U16Cmd =0;

	U8BakRxBuf = U8Uart2BakRxBuf;
	
	U8Num = U8BakRxBuf[4] + 5;
	U16Tmp = U8BakRxBuf[U8Num] * 256 + U8BakRxBuf[U8Num + 1];
	U16Crc = Crc16Calc(U8BakRxBuf, U8Num);
	U16Cmd = U8BakRxBuf[3]*256+U8BakRxBuf[2];                  //�õ�16λ������
	U16Cmd&=0x00ff;  //�����ŵϵ�Э��  
	if((U16Tmp != U16Crc)||(U8Num>135))
		return;
		
	//�ƹ��״̬����
	switch(U16Cmd)
	{
		case 0x01:
			fGlobal_Uart2NoReceviceErr = U8BakRxBuf[11]&0x01;
			fGlobal_handleBarErr = (U8BakRxBuf[11]>>1)&0x01;
			fGlobal_EbsErr = (U8BakRxBuf[11]>>2)&0x01;
			fGlobal_HallorAngleErr = (U8BakRxBuf[11]>>3)&0x01;
			fGlobal_MuchCurrent = (U8BakRxBuf[11]>>4)&0x01;
			fGlobal_StaticCurrentErr = (U8BakRxBuf[11]>>5)&0x01;
			fGlobal_BatteryErr = (U8BakRxBuf[11]>>6)&0x01;
            fGlobal_BatteryOverlowLim = (U8BakRxBuf[11]>>7)&0x01;

		break;
		default:
			
		break;	
	}
}




/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    UART2_SendData
  * @brief   ���ڷ�������
  * @param   : [����/��] 
  * @retval
  * @author  
  * @Data    2023-03-14
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void Uart2_SendData(void)
{
	static u8 U8TxBuf[UART2_TX_BUFF_SIZE],U8Num = 0;
	u16 U16Crc = 0;
	//�����������
	U8TxBuf[0] = 0x5A;
	U8TxBuf[1] = 0xB1;    //�豸��ַ
	U8TxBuf[2] = 0x01;    //������
	U8TxBuf[3] = 0x00;
	U8TxBuf[4] = 8;
	U8TxBuf[5] = U16Global_AdcPower>>8;
	U8TxBuf[5] |= (U8Global_DoubleMode<<7);
	U8TxBuf[6] = U16Global_AdcPower&0xff;
	U8TxBuf[7] |= ((U8Global_F_or_B_Mode&0x01)<<7);	
	U8TxBuf[7] |= ((fGlobal_Break&0x01)<<6);	
	U8TxBuf[7] |= ((fGlobal_WalkMode&0x01)<<5);
	U8TxBuf[7] |= ((fGlobalBrakePort&0x01)<<3);
	U8TxBuf[8] = fGlobal_SpeedMode&0x0f;     
		
	U8Num = U8TxBuf[4]+5;
	
	U16Crc = Crc16Calc(U8TxBuf, U8Num); //�õ�֡ͷ���豸��ַ�������룬���ݳ��ȣ�����CRC16
	U8TxBuf[U8Num++] = U16Crc >> 8;
	U8TxBuf[U8Num++] = U16Crc & 0xff;
	Uart2SendData(U8TxBuf, U8Num);
	
}
