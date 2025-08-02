/*基于有头链表，链头不计入数据*/

#include "hxzp_led.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"

static List* LedList;


void StartLedTask(void *argument)
{
  static uint8_t u8count = 0;

  Led *self;
  
  for(;;)
  {
    for(List *node = LedList->next; node != NULL; node = node->next)
    {
      self = (Led*)(node->data);

      /*计算下一动作时间*/
      if(self->preTime == 0)
      {
        self->busy = 1;

        self->preTime = HAL_GetTick();
      }
      else if(HAL_GetTick() - self->preTime > self->Config[self->tableNum].scanTime*10)
      {
          if(self->Config[self->tableNum].table[self->scanNum + 1] == '\0')
          {
            if(self->Config[self->tableNum].loop == 1)
            {
              self->scanNum = 0;
              self->busy = 1;
            }
            else
            {
              if(self->insertUse == 1)
              {
                self->tableNum = self->bufferNum;
                self->insertUse = 0;
                                
              }
              
              self->busy = 0;   

              continue;
            }
          }
          else if(self->Config[self->tableNum].table[self->scanNum + 1] != '\0')
          {
            self->scanNum++; 
            self->busy = 1;
          }
          
          self->preTime = HAL_GetTick();
      }  
      
      /*解析数据*/
      switch(self->Config[self->tableNum].table[self->scanNum])
      {
        case '0':
          self->light = 0;
          break;

        case '1':
          self->light = 1;
          break;
        
        case '2':
          self->light = 2;
          break;

        case '3':
          self->light = 3;
          break;
        
        case '4':
          self->light = 4;
          break;

        case '5':
          self->light = 5;
          break;

        case '6':
          self->light = 6;
          break;

        case '7':
          self->light = 7;
          break;        

        case '8':
          self->light = 8;
          break;

        case '9':
          self->light = 9;
          break;

        case 'A':
          self->light = 10;
          break;    
         
        default:
          self->light = 0xF;
          if(self->Extern != NULL)
          {
            self->Extern((char *)(&self->Config[self->tableNum].table[self->scanNum]));
          }          
          break;
      }
      
      /*执行*/
      if(self->light <= u8count && self->light <= 10 && self->Write != NULL)
      {
        self->Write(1);
      }
      else
      {
        self->Write(0);
      }
      
      u8count = (u8count == 10) ? 0 : (u8count + 1);
    }       

    osDelay(1);
  }
}


osThreadId_t LedTaskHandle;
const osThreadAttr_t LedTask_attributes = {
  .name = "LedTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

uint8_t hxzp_Led_init(Led *self)
{
  Led *temp;
  
  if(LedList == NULL)
  {
    LedList = List_Create();
  }
 
  for(List *node = LedList->next; node != NULL; node = node->next)
  {
    temp = (Led*)(node->data);
    
    if(strcmp(temp->name, self->name) == 0)
    {  
      return 0;
    }
  }  
      
  List_Insert(LedList,self);  
   
  if(LedTaskHandle == NULL && LedList != NULL)
  {
    LedTaskHandle = osThreadNew(StartLedTask, NULL, &LedTask_attributes);
  }
  
  return 1;
}

void hxzp_Led_Deinit(void)
{
  List_Destroy(LedList);
}


void hxzp_Led_set(const char *name,uint8_t num)
{
  Led *self;

  if(LedList == NULL)
  {
    return;
  }
  
  for(List *node = LedList->next; node != NULL; node = node->next)
  {
    self = (Led*)(node->data);
    
    if(strcmp(self->name, name) == 0)
    {
      if(self->ConfigNum > num)
      {
        if(self->Config[self->tableNum].priority <= self->Config[num].priority)
        {
          self->tableNum = num;
          self->scanNum = 0;
          self->preTime = 0;
        }
      }
      break;
    }
  }
}

void hxzp_Led_insert(const char *name,uint8_t num)
{
  Led *self;
  
  if(LedList == NULL)
  {
    return;
  }
  
  for(List *node = LedList->next; node != NULL; node = node->next)
  {
    self = (Led*)(node->data);
    
    if(strcmp(self->name, name) == 0)
    {
      if(self->ConfigNum > num)
      {  
        if(self->Config[self->tableNum].priority <= self->Config[num].priority)
        {        
          if(self->Config[num].loop == 0)
          {
            self->insertUse = 1;
            self->bufferNum = self->tableNum;
            self->tableNum = num;
            self->scanNum = 0; 
            self->preTime = 0;
          }
        }          
      }
      break;
    }
  }  
}  

/*可以直接一段*/
void hxzp_Led_piece(const char *name,const char *light,uint8_t sacnTime,uint8_t priority,uint8_t loop,uint8_t insert)
{
  Led *self;
  uint8_t len;
  
  if (name == NULL || light == NULL) 
  {
      return; 
  }  

  if(LedList == NULL)
  {
    return;
  }
  
  len = strlen(light);
  for(List *node = LedList->next; node != NULL; node = node->next)
  {
    self = (Led*)(node->data);
    
    if(strcmp(self->name, name) == 0)
    {
      if(self->Config[self->tableNum].priority <= priority)
      {        
        memcpy(self->Config[self->ConfigNum-1].table, light, len);
        self->Config[self->ConfigNum-1].table[len] = '\0';
        
        self->Config[self->ConfigNum-1].scanTime = sacnTime;
        self->Config[self->ConfigNum-1].priority = priority;
        self->Config[self->ConfigNum-1].loop = loop;

        if(insert && self->Config[self->ConfigNum-1].loop == 0)
        {
          self->insertUse = 1;
          self->bufferNum = self->tableNum;
          self->tableNum = self->ConfigNum-1;
          self->scanNum = 0; 
          self->preTime = 0;
        }
        else
        {
          self->insertUse = 1;
          self->tableNum = self->ConfigNum-1;
          self->scanNum = 0; 
          self->preTime = 0;   
        }
      } 

      break;
    }
  }
}




