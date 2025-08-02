

#include "FreeRTOS.h"
#include "hxzp_list.h"
#include "cmsis_os.h"
#include "main.h"

#include "hxzp_led.h"
#include "hxzp_key.h"

#include "spi.h"
#include "usart.h"

#include "fatfs.h"
#include "sdio.h"

#include "usb_device.h"
//#include "usbd_cdc_if.h"

#include "system.h"

#include "task.h"

#include <stdlib.h>

#include <stdio.h>

#include "hxzp_st7789.h"
//#include "WouoUI_user.h"
//#include "WouoUI.h"

#include "usbd_hid.h"
/*

led设计 通过外部传入亮度表以及读表速度 加入优先级 加入在忙状态

*/
extern USBD_HandleTypeDef hUsbDeviceFS;

//typedef struct {
//  
//  struct 
//  {
//    uint8_t left:1;
//    uint8_t right:1;
//    uint8_t middle:1;
//    uint8_t reserve:5;
//  }buttons;// ????? (bit0:???, bit1:???, bit2:?м?)
//  
//  int8_t x;          // X????? (-127??127)
//  int8_t y;          // Y????? (-127??127)
//  int8_t wheel;      // ??????? (-127??127)
//} HID_MOUSE_Report_t;
//HID_MOUSE_Report_t report;

void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */    
    
//  hxzp_Led_piece("W1","157AAAAAAAA9876543210",2,1,0,0);
//  hxzp_Led_piece("W2","157AAAAAAAA9876543210",2,1,0,0);
//  hxzp_Led_piece("W3","157AAAAAAAA9876543210",2,1,0,0);
//  hxzp_Led_piece("W4","157AAAAAAAA9876543210",2,1,0,0);
//  hxzp_Led_piece("W5","157AAAAAAAA9876543210",2,1,0,0);
//  hxzp_Led_piece("W6","157AAAAAAAA9876543210",2,1,0,0);    
  for(;;)
  {
    osDelay(2000);
    
    // 按下A键
//    uint8_t key_report[8] = {0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00};
//    USBD_HID_SendReport(&hUsbDeviceFS, key_report, 8, 0);

//    // 保持按下状态一段时间（可选）
//    HAL_Delay(50);  // 50ms按键持续时间

//    // 释放所有按键
//    memset(key_report, 0, sizeof(key_report));
//    USBD_HID_SendReport(&hUsbDeviceFS, key_report, 8, 0);
    
//	report.x = 50;  
//	  
//    uint8_t mouse_report[4] = {0x00, 20, 15, 0};
//    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)&report, 4, 1);    
    
//  uint8_t mouse_report[4] = {0}; // 初始化所有字段为0
////  
//  // 按下左键 (设置bit0)
//  mouse_report[0] = 0x01; // buttons: 左键按下
//  USBD_HID_SendReport(&hUsbDeviceFS, mouse_report, 4, 1);
//  
//  // 短暂延时 (模拟按下时间)
//  HAL_Delay(20); // 20ms是理想的点击延时
//  
//  // 释放所有按键
//  mouse_report[0] = 0x00;
//  USBD_HID_SendReport(&hUsbDeviceFS, mouse_report, 4, 1);	  
	  

	  
  }
  /* USER CODE END StartDefaultTask */
}










