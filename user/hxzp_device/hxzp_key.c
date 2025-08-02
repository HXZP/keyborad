/*基于有头链表，链头不计入数据*/

#include "hxzp_key.h"


static List* KeyList;

void StartKeyTask(void *argument)
{  
  Key *self;
  uint32_t errTime;
  uint8_t value;
  
  for(;;)
  {
    for(List *node = KeyList->next; node != NULL; node = node->next)
    {
      self = (Key*)(node->data);
      
      if(self->Read == NULL)
      {
        continue;
      }
      
      /*获取键值*/
      value = self->Read();
      
      /*消抖*/
      if(value != self->pre_value)
      {
        self->pre_value = value;
        
        self->switchTime = HAL_GetTick();
        
        continue;
      }
      
      errTime = HAL_GetTick() - self->switchTime;
      if(errTime > self->Config[self->tableNum].debouncing_Time)
      {
        self->value = value;
        
        /*事件判断*/
        if(self->value == 1)
        {
          if(self->state == KEY_IDLE)
          {
            self->state = KEY_PRESS;
          }         
          else if(self->state == KEY_PRESS && errTime > self->Config[self->tableNum].down_Time*100)
          {
            self->state = KEY_DOWN;
            
            if(HAL_GetTick() - self->clickTime < self->Config[self->tableNum].double_Time*100)
            {
              self->state = KEY_DOUBLE;
            }
            else
            {
              self->clickTime = HAL_GetTick();
            }
            
          }
          else if(self->state == KEY_DOWN && errTime > self->Config[self->tableNum].downlong_Time*100)
          {
            self->state = KEY_DOWN_LONG;
          }
          else if(self->state == KEY_DOWN_LONG && errTime > self->Config[self->tableNum].downhold_Time*100)
          {
            self->state = KEY_DOWN_HOLD;
          }                    
        }
        else if(self->value == 0)
        {
          if(self->state != KEY_IDLE && self->state != KEY_UP)
          {
            self->state = KEY_UP;
          }
          else if(self->state == KEY_UP)
          {
            self->state = KEY_IDLE;
          }
        }
      }
      
      /*事件处理*/
      if(self->pre_state != self->state && self->Key_EventAction != NULL)
      {
        self->Key_EventAction(self->state);
      }   
      
      self->pre_state = self->state;

    }
    
    osDelay(1);
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












