#include "drv_led.h"


Led_Config led1_Conf[5] = 
{
  {"0",1,0,0},
  {"A0A",2,0,1},
  {"123456789A98765432100000000000000000000000000",10,0,1},  
  {"123456789A",2,0,1},
  
  {"0",0,0,0},
};

Led_Config led2_Conf[5] = 
{
  {"0",1,0,0},
  {"AA0",2,0,1},
  {"00000000000123456789A987654321000000000000000",10,0,1},  
  {"123456789A",2,0,1},
  
  {"0",0,0,0},
};

Led_Config led3_Conf[5] = 
{
  {"0",1,0,0},  
  {"0AA",2,0,1},
  {"0000000000000000000000123456789A9876543210000",10,0,1},  
  {"123456789A",2,0,1},
  
  {"0",0,0,0},  
};

Led_Config led4_Conf[5] = 
{
  {"0",1,0,0},
  {"A0A",2,0,1},
  {"123456789A98765432100000000000000000000000000",10,0,1},  
  {"123456789A",2,0,1},
  
  {"0",0,0,0},
};

Led_Config led5_Conf[5] = 
{
  {"0",1,0,0},
  {"AA0",2,0,1},
  {"00000000000123456789A987654321000000000000000",10,0,1},  
  {"123456789A",2,0,1},
  
  {"0",0,0,0},
};

Led_Config led6_Conf[5] = 
{
  {"0",1,0,0},  
  {"0AA",2,0,1},
  {"0000000000000000000000123456789A9876543210000",10,0,1},  
  {"123456789A",2,0,1},
  
  {"0",0,0,0},  
};

void Led1_Write(uint8_t state)
{
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,state);
}

void Led2_Write(uint8_t state)
{
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,state);
}

void Led3_Write(uint8_t state)
{
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,state);
}

void Led4_Write(uint8_t state)
{
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,state);
}

void Led5_Write(uint8_t state)
{
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,state);
}

void Led6_Write(uint8_t state)
{
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,state);
}

Led led1 = 
{
  .name = "W1",
  .Config = led1_Conf,
  .ConfigNum = sizeof(led1_Conf)/sizeof(Led_Config),
  .Write  = Led1_Write,
};

Led led2 = 
{
  .name = "W2",
  .Config = led2_Conf,
  .ConfigNum = sizeof(led1_Conf)/sizeof(Led_Config),
  .Write  = Led2_Write,
};

Led led3 = 
{
  .name = "W3",
  .Config = led3_Conf,
  .ConfigNum = sizeof(led1_Conf)/sizeof(Led_Config),
  .Write  = Led3_Write,
};

Led led4 = 
{
  .name = "W4",
  .Config = led4_Conf,
  .ConfigNum = sizeof(led1_Conf)/sizeof(Led_Config),
  .Write  = Led4_Write,
};

Led led5 = 
{
  .name = "W5",
  .Config = led5_Conf,
  .ConfigNum = sizeof(led1_Conf)/sizeof(Led_Config),
  .Write  = Led5_Write,
};

Led led6 = 
{
  .name = "W6",
  .Config = led6_Conf,
  .ConfigNum = sizeof(led1_Conf)/sizeof(Led_Config),
  .Write  = Led6_Write,
};

void led_hw_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);

  /*Configure GPIO pins : LCD_LED_Pin LCD_A0_Pin LCD_RES_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);

  /*Configure GPIO pins : LCD_LED_Pin LCD_A0_Pin LCD_RES_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  
}

void Drv_Led_Init(void)
{
	led_hw_GPIO_Init();
	
	hxzp_Led_init(&led1);
	hxzp_Led_init(&led2);
	hxzp_Led_init(&led3);
	
	hxzp_Led_init(&led4);
	hxzp_Led_init(&led5);
	hxzp_Led_init(&led6);	
}
