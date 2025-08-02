#include "drv_init.h"

#include "usb_device.h"

void Drv_Init(void)
{
  Drv_Led_Init();  
  Drv_Key_Init();
    Drv_Encoder_Init();
//  Drv_Lcd_Init();
  
  Drv_Usart_Init();
  
  Drv_Usb_Init();
  
#if USING_PRINTF_CMBACKTRACE == 1  
  cm_backtrace_init(HW_NAME,HW_VERSION,SW_VERSION);
#endif
  
//  Drv_Bmi270_Init();
}

