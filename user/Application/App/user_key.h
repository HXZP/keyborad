#ifndef USER_KEY_H
#define USER_KEY_H

#include "drv_key.h"



typedef struct {
  /* 键盘修饰键状态 (bitmask) */
  uint8_t modifiers;     // 修饰键按位存储
  uint8_t reserved;      // 必须置0（HID规范要求）
  /* 普通按键数组（支持6键无冲）*/
  uint8_t keys[6];       // 当前按下的普通键（HID Usage ID）
  /* 扩展按键状态（可选）*/
  uint8_t extra_keys;    // 特殊功能键状态（按位存储）

} HID_KEYBOARD_Report_t;

typedef struct {
  /* 鼠标按键状态结构体 */
  struct {
    uint8_t left:1;      // 左键状态 (1: 按下, 0: 释放)
    uint8_t right:1;     // 右键状态 (1: 按下, 0: 释放)
    uint8_t middle:1;    // 中键状态 (1: 按下, 0: 释放)
    uint8_t reserve:5;   // 保留位
  }buttons;              // 鼠标按键状态

  int8_t x;              // X轴位移 (-127至127)
  int8_t y;              // Y轴位移 (-127至127)
  int8_t wheel;          // 滚轮位移 (-127至127)
} HID_MOUSE_Report_t;



void User_Key_Init(void);





#endif

