#include "AllFile.h"


/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    main
  * @brief  
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
int main(void)
{
	static u8 U8_5MsCnt = 0,U8_10MsCnt = 0,U8_50MsCnt =0;
	hardware_init();    //硬件初始化
//	Software_init();    //软件初始化
	u32 testtemp = 0;
	while(1)
	{
//    if (fGlobal_Uart1RxFlag)
//    {
//        Uart1ReceiveData();            //(控制器-扩展板通讯)仪表通讯
//        fGlobal_Uart1RxFlag = 0;
//    }
		if(fGlobal_1ms)
		{
			fGlobal_1ms = 0;
			if ((++U8_5MsCnt) >= 5)
			{
				U8_5MsCnt = 0;
			}
			if((++U8_10MsCnt) >= 10)
			{
				U8_10MsCnt = 0;
			}
			if((++U8_50MsCnt)>= 50)
			{
				U8_50MsCnt = 0;
			}
      		switch(U8_5MsCnt)
			{
				case 1:
					ADC_TASK();
				break;
				case 2:
          
				break;
				
				default:
				
				break;
			
			}
			switch (U8_10MsCnt)
			{
				case 1:
					KEY_TASK();
					break;
				
				default:
					break;
			}
			switch(U8_50MsCnt)
			{
				case 1:
//					Uart2_SendData();
				break;
				case 2:
          IO_handle();
				break;
				
				default:
				
				break;
			
			}	
		}	
	}
	return 0;
}

