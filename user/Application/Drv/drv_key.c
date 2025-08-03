#include "drv_key.h"

void key_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  /* Configure GPIO pins in INPUT PULLUP mode */
  // PE15 (Key1)
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // PB10 (Key2)
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // PB11 (Key3)
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // PE14 (Key4)
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // PE13 (Key5)
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // PE12 (Key6)
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // PE11 (Key7)
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // PE10 (Key8)
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // PE9 (Key9)
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // PA2 (Key10)
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // PA1 (Key11)
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // PA0 (Key12 - Optional)
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  
  
  
  
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_12;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);  
  
}

Key_Config key_config_default = 
{
  .debouncing_Time = 5,
  
  .down_Time = 1,
  .downlong_Time = 5,
  .downhold_Time = 50,
  .double_Time = 10, 

};

//PE15 PB10 PB11 PE14 PE13 PE12 PE11 PE10 PE9 PE8 PA2 PA1 PA0
uint8_t Key_Read_GPIO1(void) 
{
  return !HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15);
}

uint8_t Key_Read_GPIO2(void) 
{
  return !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10);
}

uint8_t Key_Read_GPIO3(void) 
{
  return !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);
}

uint8_t Key_Read_GPIO4(void) 
{
  return !HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14);
}

uint8_t Key_Read_GPIO5(void) 
{
  return !HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13);
}

uint8_t Key_Read_GPIO6(void) 
{
  return !HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_12);
}

uint8_t Key_Read_GPIO7(void) 
{
  return !HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11);
}

uint8_t Key_Read_GPIO8(void) 
{
  return !HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_10);
}

uint8_t Key_Read_GPIO9(void) 
{
  return !HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9);
}

uint8_t Key_Read_GPIO10(void) 
{
  return !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2);
}

uint8_t Key_Read_GPIO11(void) 
{
  return !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
}

uint8_t Key_Read_GPIO12(void) 
{
  return !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
}

uint8_t Key_Read_GPIOK1(void) 
{
  return !HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2);
}

uint8_t Key_Read_GPIOK2(void) 
{
  return !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12);
}

uint8_t Key_Read_GPIOK3(void) 
{
  return !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
}


Key key_config[] = 
{
{
	  .name = "Key1",
	  
	  .Config = &key_config_default,
	  .ConfigNum = sizeof(key_config_default)/sizeof(Key_Config),
		
	  .Read = Key_Read_GPIO1,	
	},
{
	  .name = "Key2",
	  
	  .Config = &key_config_default,
	  .ConfigNum = sizeof(key_config_default)/sizeof(Key_Config),
		
	  .Read = Key_Read_GPIO2,	
	},
{
	  .name = "Key3",
	  
	  .Config = &key_config_default,
	  .ConfigNum = sizeof(key_config_default)/sizeof(Key_Config),
		
	  .Read = Key_Read_GPIO3,	
	},
{
	  .name = "Key4",
	  
	  .Config = &key_config_default,
	  .ConfigNum = sizeof(key_config_default)/sizeof(Key_Config),
		
	  .Read = Key_Read_GPIO4,	
	},
{
	  .name = "Key5",
	  
	  .Config = &key_config_default,
	  .ConfigNum = sizeof(key_config_default)/sizeof(Key_Config),
		
	  .Read = Key_Read_GPIO5,	
	},
{
	  .name = "Key6",
	  
	  .Config = &key_config_default,
	  .ConfigNum = sizeof(key_config_default)/sizeof(Key_Config),
		
	  .Read = Key_Read_GPIO6,	
	},
{
	  .name = "Key7",
	  
	  .Config = &key_config_default,
	  .ConfigNum = sizeof(key_config_default)/sizeof(Key_Config),
		
	  .Read = Key_Read_GPIO7,	
	},
{
	  .name = "Key8",
	  
	  .Config = &key_config_default,
	  .ConfigNum = sizeof(key_config_default)/sizeof(Key_Config),
		
	  .Read = Key_Read_GPIO8,	
	},
{
	  .name = "Key9",
	  
	  .Config = &key_config_default,
	  .ConfigNum = sizeof(key_config_default)/sizeof(Key_Config),
		
	  .Read = Key_Read_GPIO9,	
	},
{
	  .name = "Key10",
	  
	  .Config = &key_config_default,
	  .ConfigNum = sizeof(key_config_default)/sizeof(Key_Config),
		
	  .Read = Key_Read_GPIO10,	
	},
{
	  .name = "Key11",
	  
	  .Config = &key_config_default,
	  .ConfigNum = sizeof(key_config_default)/sizeof(Key_Config),
		
	  .Read = Key_Read_GPIO11,	
	},
{
	  .name = "Key12",
	  
	  .Config = &key_config_default,
	  .ConfigNum = sizeof(key_config_default)/sizeof(Key_Config),
		
	  .Read = Key_Read_GPIO12,	
	},

{
	  .name = "K1",
	  
	  .Config = &key_config_default,
	  .ConfigNum = sizeof(key_config_default)/sizeof(Key_Config),
		
	  .Read = Key_Read_GPIOK1,	
	},
{
	  .name = "K2",
	  
	  .Config = &key_config_default,
	  .ConfigNum = sizeof(key_config_default)/sizeof(Key_Config),
		
	  .Read = Key_Read_GPIOK2,	
	},
{
	  .name = "K3",
	  
	  .Config = &key_config_default,
	  .ConfigNum = sizeof(key_config_default)/sizeof(Key_Config),
		
	  .Read = Key_Read_GPIOK3,	
	},    
};

void Drv_Key_Init(void)
{
    key_GPIO_Init();
    
	for(int i = 0; i < sizeof(key_config)/sizeof(Key); i++)
	{
		hxzp_Key_init(&key_config[i]);
	}
	
//  hxzp_Key_init(&key_cw);
//  hxzp_Key_init(&key_ccw);
//  hxzp_Key_init(&key_push);
//  hxzp_Key_init(&key_key1);
//  hxzp_Key_init(&key_key2);  
}












