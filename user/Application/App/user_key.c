#include "user_key.h"
#include "usbd_hid.h"
#include "drv_decoder.h"

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
#define KEY_RIGHT     0x4F
#define KEY_LEFT      0x50
#define KEY_DOWN      0x51
#define KEY_UP        0x52
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


HID_KEYBOARD_Report_t keyboard_report = {0};
extern USBD_HandleTypeDef hUsbDeviceFS;

static uint8_t add_keycode(uint8_t key) {
    for(int i = 0; i < 6; i++) {
        if(keyboard_report.keys[i] == 0) {
            keyboard_report.keys[i] = key;
            return 0;
        }
    }
    return 1;
}

static void remove_keycode(uint8_t key) {
    for(int i = 0; i < 6; i++) {
        if(keyboard_report.keys[i] == key) {
            keyboard_report.keys[i] = 0;
            // 保持数组紧凑
            memmove(&keyboard_report.keys[i], 
                    &keyboard_report.keys[i+1],
                    5 - i);
            keyboard_report.keys[5] = 0;
            break;
        }
    }
}

static void remove_keycode_all(void) {
    for(int i = 0; i < 6; i++) {
        keyboard_report.keys[i] = 0;
    }
}

static uint8_t ctrl_ref_cnt;
static uint8_t add_keyctrl(uint8_t key) {
    
    ctrl_ref_cnt++;
    remove_keycode_all();
    keyboard_report.modifiers = KEY_MOD_LCTRL;  // 按下左Ctrl
    keyboard_report.keys[0] = key; 
    return 0;
}

static uint8_t remove_keyctrl(uint8_t key) {
    
    ctrl_ref_cnt--;
    remove_keycode_all();
    
    keyboard_report.modifiers = 0;  // 按下左Ctrl
    
    keyboard_report.keys[0] = 0; 
    return 0;
}

// 发送鼠标报告
uint8_t USBD_HID_SendMouseReport(HID_MOUSE_Report_t *report)
{
  // 调用USBD_HID_SendReport函数，发送鼠标报告
  return USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)report, sizeof(HID_MOUSE_Report_t), 1);
}
uint8_t USBD_HID_SendKeyboardReport(HID_KEYBOARD_Report_t *report)
{
  // 调用USBD_HID_SendReport函数，发送键盘报告
  return USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)report, sizeof(HID_KEYBOARD_Report_t), 0);
}


#define DEFINE_KEY_EVENT_HANDLER(key_id, key) \
void Key_Event_##key_id(uint8_t state)       \
{                                            \
    switch(state)                            \
    {                                        \
        case KEY_IDLE:                       \
            remove_keycode(key);             \
            break;                           \
                                             \
        case KEY_PRESS:                      \
            add_keycode(key);               \
            hxzp_Led_piece("W6","00000000000000000000157AAAAAA9876543210",2,1,0,0);\
            hxzp_Led_piece("W5","0000000000000000157AAAAAA98765432100000",2,1,0,0);\
            hxzp_Led_piece("W4","000000000000157AAAAAA987654321000000000",2,1,0,0);\
            hxzp_Led_piece("W3","00000000157AAAAAA9876543210000000000000",2,1,0,0);\
            hxzp_Led_piece("W2","0000157AAAAAA98765432100000000000000000",2,1,0,0);\
            hxzp_Led_piece("W1","157AAAAAA987654321000000000000000000000",2,1,0,0);\
            break;                          \
    }                                        \
}

// 生成Key1~Key12的事件处理函数
DEFINE_KEY_EVENT_HANDLER(1, KEY_KP_7)
DEFINE_KEY_EVENT_HANDLER(2, KEY_KP_8)
DEFINE_KEY_EVENT_HANDLER(3, KEY_KP_9)
DEFINE_KEY_EVENT_HANDLER(4, KEY_KP_4)
DEFINE_KEY_EVENT_HANDLER(5, KEY_KP_5)
DEFINE_KEY_EVENT_HANDLER(6, KEY_KP_6)
DEFINE_KEY_EVENT_HANDLER(7, KEY_KP_1)
DEFINE_KEY_EVENT_HANDLER(8, KEY_KP_2)
DEFINE_KEY_EVENT_HANDLER(9, KEY_KP_3)
DEFINE_KEY_EVENT_HANDLER(10, KEY_KP_0)
DEFINE_KEY_EVENT_HANDLER(11, KEY_A)
DEFINE_KEY_EVENT_HANDLER(12, KEY_NUMLOCK)


void Key_Event_K1(uint8_t state)       
{                                            
    switch(state)                            
    {                                        
        case KEY_IDLE:                       
//            remove_keycode(key);             
            break;                           
                                             
        case KEY_PRESS:                      
//            add_keycode(key);               
            hxzp_Led_piece("W6","00000000157AAAAAA9876543210000000000000",2,1,0,0);
            hxzp_Led_piece("W5","0000157AAAAAA98765432100000000000000000",2,1,0,0);
            hxzp_Led_piece("W4","157AAAAAA987654321000000000000000000000",2,1,0,0);
            hxzp_Led_piece("W3","157AAAAAA987654321000000000000000000000",2,1,0,0);
            hxzp_Led_piece("W2","0000157AAAAAA98765432100000000000000000",2,1,0,0);
            hxzp_Led_piece("W1","00000000157AAAAAA9876543210000000000000",2,1,0,0);
            break;                          
    }                                        
}

void Key_Event_K2(uint8_t state)       \
{                                            
    switch(state)                            
    {                                        
        case KEY_IDLE:                       
//            remove_keycode(key);             
            break;                           
                                             
        case KEY_PRESS:                      
//            add_keycode(key);               
            hxzp_Led_piece("W6","00000000157AAAAAA9876543210000000000000",2,1,0,0);
            hxzp_Led_piece("W5","0000157AAAAAA98765432100000000000000000",2,1,0,0);
            hxzp_Led_piece("W4","157AAAAAA987654321000000000000000000000",2,1,0,0);
            hxzp_Led_piece("W3","157AAAAAA987654321000000000000000000000",2,1,0,0);
            hxzp_Led_piece("W2","0000157AAAAAA98765432100000000000000000",2,1,0,0);
            hxzp_Led_piece("W1","00000000157AAAAAA9876543210000000000000",2,1,0,0);
            break;                          
    }                                        
}

void Key_Event_K3(uint8_t state)       \
{                                            
    switch(state)                            
    {                                        
        case KEY_IDLE:                       
//            remove_keycode(key);             
            break;                           
                                             
        case KEY_PRESS:                      
//            add_keycode(key);               
            hxzp_Led_piece("W6","00000000157AAAAAA9876543210000000000000",2,1,0,0);
            hxzp_Led_piece("W5","0000157AAAAAA98765432100000000000000000",2,1,0,0);
            hxzp_Led_piece("W4","157AAAAAA987654321000000000000000000000",2,1,0,0);
            hxzp_Led_piece("W3","157AAAAAA987654321000000000000000000000",2,1,0,0);
            hxzp_Led_piece("W2","0000157AAAAAA98765432100000000000000000",2,1,0,0);
            hxzp_Led_piece("W1","00000000157AAAAAA9876543210000000000000",2,1,0,0);
            break;                          
    }                                        
}

int32_t last_decoder_cnt[3] = {0};
uint8_t last_decoder_dir[3] = {0};


void User_Decoder_Poll(void)
{
    if(Encoder_GetDirection(0) == ENCODER_DIR_CW)
    {
        if(last_decoder_cnt[0] != Encoder_GetCounter(0))
            hxzp_Led_piece("W6","A0A0",1,1,0,0);
        
//        remove_keycode_all();
//        add_keycode(KEY_BACKSPACE);            
    }
    else if(Encoder_GetDirection(0) == ENCODER_DIR_CCW)
    {
        if(last_decoder_cnt[0] != Encoder_GetCounter(0))
            hxzp_Led_piece("W5","A0A0",1,1,0,0);
        
//        add_keyctrl(KEY_Z);
    }
    else
    {
//        if(last_decoder_dir[0] != ENCODER_DIR_NONE)
//        {
//            remove_keycode_all();
//        }
    }
    last_decoder_dir[0] = Encoder_GetDirection(0);
    last_decoder_cnt[0] = Encoder_GetCounter(0);

    
    if(Encoder_GetDirection(1) == ENCODER_DIR_CW)
    {
        if(last_decoder_cnt[1] != Encoder_GetCounter(1))
            hxzp_Led_piece("W4","A0A0",1,1,0,0);          
    }
    else if(Encoder_GetDirection(1) == ENCODER_DIR_CCW)
    {
        if(last_decoder_cnt[1] != Encoder_GetCounter(1))
            hxzp_Led_piece("W3","A0A0",1,1,0,0);
    }
    else
    {

    }
    last_decoder_dir[1] = Encoder_GetDirection(1);
    last_decoder_cnt[1] = Encoder_GetCounter(1);
    
    if(Encoder_GetDirection(2) == ENCODER_DIR_CW)
    {
        if(last_decoder_cnt[2] != Encoder_GetCounter(2))
            hxzp_Led_piece("W2","A0A0",1,1,0,0);          
    }
    else if(Encoder_GetDirection(2) == ENCODER_DIR_CCW)
    {
        if(last_decoder_cnt[2] != Encoder_GetCounter(2))
            hxzp_Led_piece("W1","A0A0",1,1,0,0);
    }
    else
    {

    }
    last_decoder_dir[2] = Encoder_GetDirection(2);
    last_decoder_cnt[2] = Encoder_GetCounter(2);    
}




osThreadId_t endoerTaskHandle;
const osThreadAttr_t endoerTask_attributes = {
  .name = "endoerTask",
  .stack_size = 128 * 1,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t keyboradTaskHandle;
const osThreadAttr_t keyboradTask_attributes = {
  .name = "keyboradTask",
  .stack_size = 128 * 1,
  .priority = (osPriority_t) osPriorityNormal,
};

uint8_t num_init = 0;
void StartkeyboradTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */    
  for(;;)
  {
    if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED) 
    {
        if(num_init == 0)   
        {
            hxzp_Led_piece("W1","157AAAAAAAA9876543210",2,1,0,0);
            hxzp_Led_piece("W2","157AAAAAAAA9876543210",2,1,0,0);
            hxzp_Led_piece("W3","157AAAAAAAA9876543210",2,1,0,0);
            hxzp_Led_piece("W4","157AAAAAAAA9876543210",2,1,0,0);
            hxzp_Led_piece("W5","157AAAAAAAA9876543210",2,1,0,0);
            hxzp_Led_piece("W6","157AAAAAAAA9876543210",2,1,0,0);             
            num_init = 1;   
        }
        USBD_HID_SendKeyboardReport(&keyboard_report);
    } 
    else 
    {
        num_init = 0;
        Encoder_ResetCounterAll();
    }
    
    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}
void StartendoerTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */    
  for(;;)
  {
    if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED) 
    {
        Encoder_PollAll();
        User_Decoder_Poll();
    } 
    else 
    {
        Encoder_ResetCounterAll();
    }
    
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}
void User_Key_Init(void)
{
    hxzp_Key_eventReg("Key1", Key_Event_1);
    hxzp_Key_eventReg("Key2", Key_Event_2);
    hxzp_Key_eventReg("Key3", Key_Event_3);
    hxzp_Key_eventReg("Key4", Key_Event_4);
    hxzp_Key_eventReg("Key5", Key_Event_5);
    hxzp_Key_eventReg("Key6", Key_Event_6);
    hxzp_Key_eventReg("Key7", Key_Event_7);
    hxzp_Key_eventReg("Key8", Key_Event_8);
    hxzp_Key_eventReg("Key9", Key_Event_9);
    hxzp_Key_eventReg("Key10", Key_Event_10);
    hxzp_Key_eventReg("Key11", Key_Event_11);
    hxzp_Key_eventReg("Key12", Key_Event_12);

    hxzp_Key_eventReg("K1", Key_Event_K1);
    hxzp_Key_eventReg("K2", Key_Event_K2);
    hxzp_Key_eventReg("K3", Key_Event_K3);
    
    keyboradTaskHandle = osThreadNew(StartkeyboradTask, NULL, &keyboradTask_attributes);
    keyboradTaskHandle = osThreadNew(StartendoerTask, NULL, &endoerTask_attributes);
}












