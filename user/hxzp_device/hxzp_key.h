/*
* 多按键触发：读值的时候同时读取两个按键的键值与结果
* 
**/


#ifndef HXZP_KEY_H
#define HXZP_KEY_H

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "string.h"
#include "hxzp_list.h"
#include "main.h"

typedef enum
{
  KEY_IDLE = 0,
  
  KEY_PRESS,
  KEY_DOWN,
  KEY_DOWN_LONG,
  KEY_DOWN_HOLD,
  KEY_UP,
  KEY_DOUBLE,
  
}Key_State_e; 


typedef struct hxzp_Key_Config_s{

  /*1:1ms*/
  uint8_t  debouncing_Time;
  
  /*1:100ms*/
  uint8_t  down_Time;
  uint16_t downlong_Time;
  uint16_t downhold_Time; 
  uint8_t  double_Time;  
  
}Key_Config;



typedef struct hxzp_Key_s{

  char *name;
    
  Key_Config *Config;

  uint32_t switchTime;
  uint32_t clickTime;  
  struct
  {
    uint16_t     value : 1;
    uint16_t pre_value : 1;
    uint16_t     state : 4;
    uint16_t pre_state : 4;
    uint16_t ConfigNum : 3;
    uint16_t  tableNum : 3;    
  };
  
  uint8_t (*Read)(void);
  void    (*Key_EventAction)(uint8_t state);
  
  void    (*Extern)(void);

}Key;

typedef void (*key_event)(uint8_t state);

uint8_t hxzp_Key_init(Key *self);
  
void hxzp_Key_eventReg(const char *name, key_event handle);

void hxzp_Key_modifyConfig(const char *name, uint8_t configID, Key_Config *config);

uint8_t hxzp_Key_getValue(const char *name);









#endif
