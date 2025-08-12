#include "user_key.h"
#include "usbd_hid.h"
#include "drv_decoder.h"

uint8_t keyboard_lock = 0;
HID_KEYBOARD_Report_t keyboard_report = {0};
extern USBD_HandleTypeDef hUsbDeviceFS;

static uint8_t add_keycode(uint8_t key) {

    if(keyboard_lock) { // 锁定状态，不处理
        return 1;
    }
    
    for(int i = 0; i < 6; i++) {
        if(keyboard_report.keys[i] == 0) {
            keyboard_report.keys[i] = key;
            return 0;
        }
    }
    return 1;
}

static void add_2keycode(uint8_t key1,uint8_t key2)
{
    add_keycode(key1);
    add_keycode(key2);
}

static void remove_keycode(uint8_t key) {

    if(keyboard_lock) { // 锁定状态，不处理
        return;
    }
    
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

    keyboard_lock = 0;
    memset(&keyboard_report, 0 ,sizeof(keyboard_report));
}

static uint8_t ctrl_ref_cnt;
static uint8_t add_keymodifiers(uint8_t modifiers, uint8_t key) {
    
    if(keyboard_lock) { // 锁定状态，不处理
        return 1;
    }
    
    keyboard_lock = 1;

    ctrl_ref_cnt++;
    memset(&keyboard_report, 0 ,sizeof(keyboard_report));
    keyboard_report.modifiers = modifiers;  // 按下左Ctrl
    for(int i = 0; i < 6; i++) {
        if(keyboard_report.keys[i] == 0) {
            keyboard_report.keys[i] = key;
            return 0;
        }
    }
    return 1;
}

static uint8_t add_2keymodifiers(uint8_t modifiers, uint8_t key1,uint8_t key2) {
    
    if(keyboard_lock) { // 锁定状态，不处理
        return 1;
    }
    
    keyboard_lock = 1;

    ctrl_ref_cnt++;
    memset(&keyboard_report, 0 ,sizeof(keyboard_report));
    keyboard_report.modifiers = modifiers;  // 按下左Ctrl
    for(int i = 0; i < 6; i++) {
        if(keyboard_report.keys[i] == 0) {
            keyboard_report.keys[i] = key1;
            keyboard_report.keys[i+1] = key2;
            return 0;
        }
    }
    return 1;
}

// static uint8_t remove_keymodifiers(uint8_t modifiers, uint8_t key) {
    
//     ctrl_ref_cnt--;
//     memset(&keyboard_report, 0 ,sizeof(keyboard_report));
//     keyboard_report.modifiers = modifiers;  // 按下左Ctrl
//     for(int i = 0; i < 6; i++) {
//         if(keyboard_report.keys[i] == key) {
//             keyboard_report.keys[i] = 0;
//             // 保持数组紧凑
//             memmove(&keyboard_report.keys[i], 
//                     &keyboard_report.keys[i+1],
//                     5 - i);
//             keyboard_report.keys[5] = 0;
//             break;
//         }
//     }
//     return 0;
// }




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

#define DEFINE_KEY_MOD_EVENT_HANDLER(key_id, MOD) \
void Key_Event_##key_id(uint8_t state)       \
{                                            \
    switch(state)                            \
    {                                        \
        case KEY_IDLE:                       \
            remove_keycode_all();                   \
            break;                           \
                                             \
        case KEY_DOWN:                      \
            MOD;               \
            hxzp_Led_piece("W6","00000000000000000000157AAAAAA9876543210",2,1,0,0);\
            hxzp_Led_piece("W5","0000000000000000157AAAAAA98765432100000",2,1,0,0);\
            hxzp_Led_piece("W4","000000000000157AAAAAA987654321000000000",2,1,0,0);\
            hxzp_Led_piece("W3","00000000157AAAAAA9876543210000000000000",2,1,0,0);\
            hxzp_Led_piece("W2","0000157AAAAAA98765432100000000000000000",2,1,0,0);\
            hxzp_Led_piece("W1","157AAAAAA987654321000000000000000000000",2,1,0,0);\
            break;                          \
    }                                        \
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
        case KEY_DOWN:                      \
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

#define DEFINE_KEY_2EVENT_HANDLER(key_id, DOWN_UP, DOWN_LONG) \
void Key_Event_##key_id(uint8_t state)        \
{                                             \
    switch(state)                             \
    {                                         \
        case KEY_IDLE:                        \
            remove_keycode_all();         \
            break;                            \
 \
        case KEY_DOWN_UP:       \
            remove_keycode_all();       \
            DOWN_UP; \
            hxzp_Led_piece("W6","00000000000000000000157AAAAAA9876543210",2,1,0,0); \
            hxzp_Led_piece("W5","0000000000000000157AAAAAA98765432100000",2,1,0,0); \
            hxzp_Led_piece("W4","000000000000157AAAAAA987654321000000000",2,1,0,0); \
            hxzp_Led_piece("W3","00000000157AAAAAA9876543210000000000000",2,1,0,0); \
            hxzp_Led_piece("W2","0000157AAAAAA98765432100000000000000000",2,1,0,0); \
            hxzp_Led_piece("W1","157AAAAAA987654321000000000000000000000",2,1,0,0); \
            break;      \
             \
        case KEY_DOWN_LONG: \
            remove_keycode_all();       \
            DOWN_LONG; \
            hxzp_Led_piece("W6","00000000000000000000157AAAAAA9876543210",2,1,0,0); \
            hxzp_Led_piece("W5","0000000000000000157AAAAAA98765432100000",2,1,0,0); \
            hxzp_Led_piece("W4","000000000000157AAAAAA987654321000000000",2,1,0,0); \
            hxzp_Led_piece("W3","00000000157AAAAAA9876543210000000000000",2,1,0,0); \
            hxzp_Led_piece("W2","0000157AAAAAA98765432100000000000000000",2,1,0,0); \
            hxzp_Led_piece("W1","157AAAAAA987654321000000000000000000000",2,1,0,0); \
        break; \
    }                        \
} 

// 生成Key1~Key12的事件处理函数
DEFINE_KEY_2EVENT_HANDLER(1, add_keycode(KEY_KP_DOT),add_2keymodifiers(KEY_MOD_LSHIFT,KEY_KP_MINUS,KEY_PERIOD))
DEFINE_KEY_EVENT_HANDLER(2, KEY_NUMLOCK)
DEFINE_KEY_EVENT_HANDLER(3, KEY_KP_PLUS)
DEFINE_KEY_2EVENT_HANDLER(4, add_2keycode(KEY_KP_SLASH,KEY_KP_ASTERISK),add_2keycode(KEY_KP_ASTERISK,KEY_KP_SLASH))
DEFINE_KEY_EVENT_HANDLER(5, KEY_F1)
DEFINE_KEY_EVENT_HANDLER(6, KEY_MINUS)
DEFINE_KEY_MOD_EVENT_HANDLER(7, add_keymodifiers(KEY_MOD_LSHIFT,0))
DEFINE_KEY_EVENT_HANDLER(8, KEY_ESC)
DEFINE_KEY_EVENT_HANDLER(9, KEY_KP_ASTERISK)
DEFINE_KEY_MOD_EVENT_HANDLER(10, add_keymodifiers(KEY_MOD_LCTRL,0))
 DEFINE_KEY_EVENT_HANDLER(11, KEY_ENTER)
DEFINE_KEY_EVENT_HANDLER(12, KEY_KP_SLASH)


//编码器1
void Key_Event_K1(uint8_t state)       
{                                            
    switch(state)                            
    {                                        
        case KEY_IDLE:                       
            remove_keycode_all();               
            break;                           
                                             
        case KEY_DOWN:                      
            
            add_keymodifiers(KEY_MOD_LCTRL,KEY_X);                       
            hxzp_Led_piece("W6","00000000157AAAAAA9876543210000000000000",2,1,0,0);
            hxzp_Led_piece("W5","0000157AAAAAA98765432100000000000000000",2,1,0,0);
            hxzp_Led_piece("W4","157AAAAAA987654321000000000000000000000",2,1,0,0);
            hxzp_Led_piece("W3","157AAAAAA987654321000000000000000000000",2,1,0,0);
            hxzp_Led_piece("W2","0000157AAAAAA98765432100000000000000000",2,1,0,0);
            hxzp_Led_piece("W1","00000000157AAAAAA9876543210000000000000",2,1,0,0);
            break;                          
    }                                        
}

//编码器2
void Key_Event_K2(uint8_t state)       \
{                                            
    switch(state)                            
    {                                        
        case KEY_IDLE:    
            remove_keycode_all();               
            break;                           
                                             
        case KEY_DOWN:    
            remove_keycode_all(); 
            add_keycode(KEY_F7);                
            hxzp_Led_piece("W6","00000000157AAAAAA9876543210000000000000",2,1,0,0);
            hxzp_Led_piece("W5","0000157AAAAAA98765432100000000000000000",2,1,0,0);
            hxzp_Led_piece("W4","157AAAAAA987654321000000000000000000000",2,1,0,0);
            hxzp_Led_piece("W3","157AAAAAA987654321000000000000000000000",2,1,0,0);
            hxzp_Led_piece("W2","0000157AAAAAA98765432100000000000000000",2,1,0,0);
            hxzp_Led_piece("W1","00000000157AAAAAA9876543210000000000000",2,1,0,0);
            break;                          
    }                                        
}

void Key_Event_K3(uint8_t state)       
{                                            
    switch(state)                            
    {                                        
        case KEY_IDLE:     
            remove_keycode_all();               
            break;                           
                                             
        case KEY_DOWN:    
            remove_keycode_all(); 
            add_keycode(KEY_F8);               
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
    /*编码器3*/
    if(Encoder_GetDirection(0) == ENCODER_DIR_CW)
    {
        if(last_decoder_cnt[0] != Encoder_GetCounter(0))
            hxzp_Led_piece("W6","A0A0",1,1,0,0);

        remove_keycode_all();
        add_keycode(KEY_TAB);   
    }
    else if(Encoder_GetDirection(0) == ENCODER_DIR_CCW)
    {
        if(last_decoder_cnt[0] != Encoder_GetCounter(0))
            hxzp_Led_piece("W5","A0A0",1,1,0,0);
        
        remove_keycode_all();
        add_keycode(KEY_BACKSPACE);    
    }
    else
    {
        if(last_decoder_dir[0] != Encoder_GetDirection(0))
        {
            remove_keycode_all();
        }
    }
    last_decoder_dir[0] = Encoder_GetDirection(0);
    last_decoder_cnt[0] = Encoder_GetCounter(0);

    /*编码器2*/
    if(Encoder_GetDirection(1) == ENCODER_DIR_CW)
    {
        if(last_decoder_cnt[1] != Encoder_GetCounter(1))
            hxzp_Led_piece("W4","A0A0",1,1,0,0);     
        
        add_keymodifiers(KEY_MOD_LCTRL,KEY_Y);
    }
    else if(Encoder_GetDirection(1) == ENCODER_DIR_CCW)
    {
        if(last_decoder_cnt[1] != Encoder_GetCounter(1))
            hxzp_Led_piece("W3","A0A0",1,1,0,0);
        
        add_keymodifiers(KEY_MOD_LCTRL,KEY_Z);    
    }
    else
    {
        if(last_decoder_dir[1] != Encoder_GetDirection(1))
        {
            remove_keycode_all();
        }
    }
    last_decoder_dir[1] = Encoder_GetDirection(1);
    last_decoder_cnt[1] = Encoder_GetCounter(1);
    
    /*编码器1*/
    if(Encoder_GetDirection(2) == ENCODER_DIR_CW)
    {
        if(last_decoder_cnt[2] != Encoder_GetCounter(2))
            hxzp_Led_piece("W2","A0A0",1,1,0,0);        

        add_keymodifiers(KEY_MOD_LCTRL,KEY_V);    
    }
    else if(Encoder_GetDirection(2) == ENCODER_DIR_CCW)
    {
        if(last_decoder_cnt[2] != Encoder_GetCounter(2))
            hxzp_Led_piece("W1","A0A0",1,1,0,0);

        add_keymodifiers(KEY_MOD_LCTRL,KEY_C);    
    }
    else
    {
        if(last_decoder_dir[2] != Encoder_GetDirection(2))
        {
            remove_keycode_all();
        }
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












