
#include "hxzp_key.h"


static List* KeyList;

void StartKeyTask(void *argument) {
  Key *self;
  uint32_t currentTick;
  uint8_t rawValue;
  
  for(;;) {
    currentTick = HAL_GetTick(); // 统一时间戳
    
    for(List *node = KeyList->next; node != NULL; node = node->next) {
      self = (Key*)(node->data);
      
      // 跳过无效按键
      if(self->Read == NULL) {
        continue;
      }

      // 1. 读取原始值并更新变化时间
      rawValue = self->Read();
      if(rawValue != self->pre_value) {
        self->pre_value = rawValue;
        self->switchTime = currentTick;
      }

      // 2. 计算稳定时间（注意单位转换）
      uint32_t stableTime = currentTick - self->switchTime;
      uint32_t debounceTime = self->Config[self->tableNum].debouncing_Time; // ms
      
      // 3. 跳过消抖期
      if(stableTime < debounceTime) {
        continue;
      }

      // 4. 状态机处理（修复事件丢失问题）
      uint8_t lastState = self->state;
      
      if(rawValue == 1) { // 按键按下
        switch(self->state) {
          case KEY_IDLE:
            // 按下后立即进入DOWN状态，不等待
            self->state = KEY_DOWN;
            self->pressStart = currentTick; // 记录按下时间
            break;
            
          case KEY_DOWN:
            // 检查长按条件（注意单位转换：down_Time*100 = 100ms单位）
            if(stableTime > self->Config[self->tableNum].downlong_Time * 100) {
              self->state = KEY_DOWN_LONG;
            }
            break;
            
          case KEY_DOWN_LONG:
            // 检查连发条件（downhold_Time单位100ms）
            if(stableTime > self->Config[self->tableNum].downhold_Time * 100) {
              self->state = KEY_DOWN_HOLD;
            }
            break;
            
          // 其他状态保持不变
          default: break;
        }
      } 
      else { // 按键释放
        switch(self->state) {
          case KEY_DOWN:
            self->state = KEY_DOWN_UP;
            break;
            
          case KEY_DOWN_LONG:
            self->state = KEY_DOWN_LONG_UP;
            break;
            
          case KEY_DOWN_HOLD:
            self->state = KEY_DOWN_HOLD_UP;
            break;
            
          case KEY_DOWN_UP:
          case KEY_DOWN_LONG_UP:
          case KEY_DOWN_HOLD_UP:
            self->state = KEY_IDLE;
            break;
            
          default: break;
        }
      }

      // 5. 修复事件丢失的关键：立即触发状态变化事件
      if(lastState != self->state && self->Key_EventAction != NULL) {
        self->Key_EventAction(self->state);
        
        // 特殊处理连发事件
        if(self->state == KEY_DOWN_HOLD) {
          // 重置计时器以实现周期性触发
          self->switchTime = currentTick;
        }
      }
    }
    
    osDelay(10); // 适当增加延时减少CPU负载
  }
}


osThreadId_t KeyTaskHandle;
const osThreadAttr_t KeyTask_attributes = {
  .name = "KeyTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

uint8_t hxzp_Key_init(Key *self)
{
  Key *temp;
  
  if(KeyList == NULL)
  {
    KeyList = List_Create();
  }

  for(List *node = KeyList->next; node != NULL; node = node->next)
  {
    temp = (Key*)(node->data);
    
    if(strcmp(temp->name, self->name) == 0)
    {  
      return 0;
    }
  }  
  
  List_Insert(KeyList,self);  
   
  if(KeyTaskHandle == NULL && KeyList != NULL)
  {
    KeyTaskHandle = osThreadNew(StartKeyTask, NULL, &KeyTask_attributes);
  }
  
  return 1;
}

void hxzp_Key_Deinit(void)
{
  List_Destroy(KeyList);
}


void hxzp_Key_eventReg(const char *name, key_event handle)
{
  Key *self;
  
  if(handle == NULL)
  {
    return;
  }
  
  for(List *node = KeyList->next; node != NULL; node = node->next)
  {
    self = (Key*)(node->data);
    
    if(strcmp(self->name, name) == 0)
    {
      self->Key_EventAction = handle;
            
      break;
    }
  }
}

void hxzp_Key_modifyConfig(const char *name, uint8_t configID, Key_Config *config)
{
  Key *self;
  
  if(config == NULL)
  {
    return;
  }
  
  if(self->ConfigNum > configID)
  {
    for(List *node = KeyList->next; node != NULL; node = node->next)
    {
      self = (Key*)(node->data);
      
      if(strcmp(self->name, name) == 0)
      {
        memcpy(&self->Config[configID], config,sizeof(Key_Config));
              
        break;
      }
    }
  }
}

uint8_t hxzp_Key_getValue(const char *name)
{
  Key *self;  
  
  for(List *node = KeyList->next; node != NULL; node = node->next)
  {
    self = (Key*)(node->data);
    
    if(strcmp(self->name, name) == 0)
    {
      return self->value;
    }
  }
  
  return 0xFF;
}












