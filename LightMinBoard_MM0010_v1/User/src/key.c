#include "AllFile.h"
//testbranch modify
//test git(Master)
void KEY_TASK(void)
{
  static u8 U8LastKeyStatus = 0,count = 0;
  u8 u8KeyStatus = 0;
  u8KeyStatus = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2);
  if(u8KeyStatus == 0 && U8LastKeyStatus != u8KeyStatus)
  {
    count++;
  }
  U8LastKeyStatus = u8KeyStatus;
}
