#ifndef USER_KEY_H
#define USER_KEY_H

#include "drv_key.h"


/* 修饰键状态字节（bitmask）*/
#define KEY_MOD_LCTRL     (1 << 0)   // 0x01 : 左Ctrl
#define KEY_MOD_LSHIFT    (1 << 1)   // 0x02 : 左Shift
#define KEY_MOD_LALT      (1 << 2)   // 0x04 : 左Alt (Option on Mac)
#define KEY_MOD_LGUI      (1 << 3)   // 0x08 : 左Win/Command (Windows键或Mac Command键)
#define KEY_MOD_RCTRL     (1 << 4)   // 0x10 : 右Ctrl
#define KEY_MOD_RSHIFT    (1 << 5)   // 0x20 : 右Shift
#define KEY_MOD_RALT      (1 << 6)   // 0x40 : 右Alt (AltGr或Option on Mac)
#define KEY_MOD_RGUI      (1 << 7)   // 0x80 : 右Win/Command
/* ========== 字母键 ========== */
#define KEY_A       0x04
#define KEY_B       0x05
#define KEY_C       0x06
#define KEY_D       0x07
#define KEY_E       0x08
#define KEY_F       0x09
#define KEY_G       0x0A
#define KEY_H       0x0B
#define KEY_I       0x0C
#define KEY_J       0x0D
#define KEY_K       0x0E
#define KEY_L       0x0F
#define KEY_M       0x10
#define KEY_N       0x11
#define KEY_O       0x12
#define KEY_P       0x13
#define KEY_Q       0x14
#define KEY_R       0x15
#define KEY_S       0x16
#define KEY_T       0x17
#define KEY_U       0x18
#define KEY_V       0x19
#define KEY_W       0x1A
#define KEY_X       0x1B
#define KEY_Y       0x1C
#define KEY_Z       0x1D

/* ========== 数字键（主键盘区） ========== */
#define KEY_1       0x1E  // !
#define KEY_2       0x1F  // @
#define KEY_3       0x20  // #
#define KEY_4       0x21  // $
#define KEY_5       0x22  // %
#define KEY_6       0x23  // ^
#define KEY_7       0x24  // &
#define KEY_8       0x25  // *
#define KEY_9       0x26  // (
#define KEY_0       0x27  // )

/* ========== 功能键 ========== */
#define KEY_ENTER   0x28
#define KEY_ESC     0x29
#define KEY_BACKSPACE 0x2A
#define KEY_TAB     0x2B
#define KEY_SPACE   0x2C
#define KEY_MINUS   0x2D  // _
#define KEY_EQUAL   0x2E  // +
#define KEY_LBRACE  0x2F  // {
#define KEY_RBRACE  0x30  // }
#define KEY_BACKSLASH 0x31  // |
#define KEY_SEMICOLON 0x33  // :
#define KEY_QUOTE   0x34  // "
#define KEY_TILDE   0x35  // ~
#define KEY_COMMA   0x36  // <
#define KEY_PERIOD  0x37  // >
#define KEY_SLASH   0x38  // ?

/* ========== 修饰键 ========== */
#define KEY_CAPSLOCK 0x39
#define KEY_F1      0x3A
#define KEY_F2      0x3B
#define KEY_F3      0x3C
#define KEY_F4      0x3D
#define KEY_F5      0x3E
#define KEY_F6      0x3F
#define KEY_F7      0x40
#define KEY_F8      0x41
#define KEY_F9      0x42
#define KEY_F10     0x43
#define KEY_F11     0x44
#define KEY_F12     0x45

/* ========== 控制键 ========== */
#define KEY_PRINTSCREEN 0x46
#define KEY_SCROLLLOCK 0x47
#define KEY_PAUSE     0x48
#define KEY_INSERT    0x49
#define KEY_HOME      0x4A
#define KEY_PAGEUP    0x4B
#define KEY_DELETE    0x4C
#define KEY_END       0x4D
#define KEY_PAGEDOWN  0x4E
#define KEY_RIGHT_     0x4F
#define KEY_LEFT_      0x50
#define KEY_DOWN_      0x51
#define KEY_UP_        0x52
#define KEY_NUMLOCK   0x53

/* ========== 小键盘 ========== */
#define KEY_KP_SLASH   0x54
#define KEY_KP_ASTERISK 0x55
#define KEY_KP_MINUS   0x56
#define KEY_KP_PLUS    0x57
#define KEY_KP_ENTER   0x58
#define KEY_KP_1       0x59
#define KEY_KP_2       0x5A
#define KEY_KP_3       0x5B
#define KEY_KP_4       0x5C
#define KEY_KP_5       0x5D
#define KEY_KP_6       0x5E
#define KEY_KP_7       0x5F
#define KEY_KP_8       0x60
#define KEY_KP_9       0x61
#define KEY_KP_0       0x62
#define KEY_KP_DOT     0x63

/* ========== 国际键 ========== */
#define KEY_NONUS_BACKSLASH 0x64  // \|
#define KEY_APPLICATION 0x65  // Windows右键菜单键
#define KEY_POWER      0x66
#define KEY_MENU       0x76


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

