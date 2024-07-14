#include "AllFile.h"

void ADC_TASK(void)
{
    static u32 i = 0;
    u16 testnum = 0;
    testnum = fGlobal_vu16IlluminData[0];
    if(testnum != 0)
    {
        i++;
    }
}
