
#include "drv_decoder.h"  // 根据实际MCU修改


void decoder_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  /* Configure GPIO pins in INPUT PULLUP mode */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_11;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_0;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_1;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_3;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}



Encoder_HandleTypeDef encoder[3] = {

    {
        .GPIOA_Port = GPIOC,
        .GPIOA_Pin = GPIO_PIN_11,
        .GPIOB_Port = GPIOC,
        .GPIOB_Pin = GPIO_PIN_10,
        .last_state = 0,
        .counter = 0,
        .step = 0,
    },
    {
        .GPIOA_Port = GPIOD,
        .GPIOA_Pin = GPIO_PIN_1,
        .GPIOB_Port = GPIOD,
        .GPIOB_Pin = GPIO_PIN_0,
        .last_state = 0,
        .counter = 0,
        .step = 0,
    },
    {
        .GPIOA_Port = GPIOE,
        .GPIOA_Pin = GPIO_PIN_4,
        .GPIOB_Port = GPIOE,
        .GPIOB_Pin = GPIO_PIN_3,
        .last_state = 0,
        .counter = 0,
        .step = 0,
    },
};




// 状态机转换表
// 索引格式: 旧状态AB (高2位) | 新状态AB (低2位)
// 正确的正交编码器状态转换表
static const int8_t encoder_transition_table[16] = {
    /* 00->00 */  0, /* 00->01 */  1, /* 00->10 */ -1, /* 00->11 */  0,
    /* 01->00 */ -1, /* 01->01 */  0, /* 01->10 */  0, /* 01->11 */  1,
    /* 10->00 */  1, /* 10->01 */  0, /* 10->10 */  0, /* 10->11 */ -1,
    /* 11->00 */  0, /* 11->01 */ -1, /* 11->10 */  1, /* 11->11 */  0
};

// 初始化编码器
void Encoder_Init(Encoder_HandleTypeDef *henc)
{
    // 读取初始状态
    uint8_t stateA = !HAL_GPIO_ReadPin(henc->GPIOA_Port, henc->GPIOA_Pin);
    uint8_t stateB = !HAL_GPIO_ReadPin(henc->GPIOB_Port, henc->GPIOB_Pin);
    henc->last_state = (stateA << 1) | stateB;
    
    henc->counter = 0;
    henc->step = 0;
    
    // 初始化防抖状态
    henc->confirmed_dir = ENCODER_DIR_NONE;
    henc->candidate_dir = ENCODER_DIR_NONE;
    henc->debounce_count = 0;
    henc->debounce_threshold = 5;
    henc->last_change_time = HAL_GetTick();    
}

// 轮询指定索引的编码器
void Encoder_Poll(uint8_t enc_index)
{
    if (enc_index >= sizeof(encoder)/sizeof(encoder[0])) return;
    
    Encoder_HandleTypeDef *henc = &encoder[enc_index];
    
    // 读取当前状态
    henc->stateA = !HAL_GPIO_ReadPin(henc->GPIOA_Port, henc->GPIOA_Pin);
    henc->stateB = !HAL_GPIO_ReadPin(henc->GPIOB_Port, henc->GPIOB_Pin);
    uint8_t current_state = (henc->stateA << 1) | henc->stateB;
    
    // 计算状态索引
    uint8_t transition_index = (henc->last_state << 2) | current_state;
    
    // 查表获取状态变化值
    int8_t change = encoder_transition_table[transition_index & 0x0F];
    
    // 更新计数器
    henc->counter += change;
    henc->step = change;
    
    // 保存当前状态供下次使用
    henc->last_state = current_state;
    
    // ===== 防抖处理部分 =====
    // 检测方向变化
    EncoderDirection new_dir = ENCODER_DIR_NONE;
    if (change > 0) {
        new_dir = ENCODER_DIR_CW;
    } else if (change < 0) {
        new_dir = ENCODER_DIR_CCW;
    }
    
    // 如果有方向变化
    if (new_dir != ENCODER_DIR_NONE) {
        // 方向变化时更新时间戳
        henc->last_change_time = HAL_GetTick();
        
        if (new_dir == henc->candidate_dir) {
            // 方向相同，增加防抖计数
            if (henc->debounce_count < henc->debounce_threshold) {
                henc->debounce_count++;
            }
            
            // 达到防抖阈值，确认方向
            if (henc->debounce_count >= henc->debounce_threshold) {
                henc->confirmed_dir = new_dir;
            }
        } else {
            // 方向改变，重置防抖计数
            henc->candidate_dir = new_dir;
            henc->debounce_count = 1;
        }
    } else {
        // 无变化时检查是否超时
        if ((HAL_GetTick() - henc->last_change_time) > 100) {
            henc->confirmed_dir = ENCODER_DIR_NONE;
            henc->candidate_dir = ENCODER_DIR_NONE;
            henc->debounce_count = 0;
        }
    }
}

EncoderDirection Encoder_GetDirection(uint8_t enc_index)
{
    if (enc_index >= sizeof(encoder)/sizeof(encoder[0])) 
        return ENCODER_DIR_NONE;
    
    return encoder[enc_index].confirmed_dir;
}

// 轮询所有编码器
void Encoder_PollAll(void)
{
    for (uint8_t i = 0; i < sizeof(encoder)/sizeof(encoder[0]); i++) {
        if (encoder[i].GPIOA_Port != NULL) {
            Encoder_Poll(i);
        }
    }
}

void Encoder_ResetCounterAll(void)
{
    for (uint8_t i = 0; i < sizeof(encoder)/sizeof(encoder[0]); i++) {
        encoder[i].counter = 0;
    }
}

// 获取指定编码器的计数值
int32_t Encoder_GetCounter(uint8_t enc_index)
{
    if (enc_index >= sizeof(encoder)/sizeof(encoder[0])) return 0;
    return encoder[enc_index].counter;
}

// 重置指定编码器的计数器
void Encoder_ResetCounter(uint8_t enc_index)
{
    if (enc_index < sizeof(encoder)/sizeof(encoder[0])) {
        encoder[enc_index].counter = 0;
    }
}

// 获取指定编码器的单次步数变化
int8_t Encoder_GetStep(uint8_t enc_index)
{
    if (enc_index >= sizeof(encoder)/sizeof(encoder[0])) return 0;
    
    Encoder_HandleTypeDef *henc = &encoder[enc_index];
    int8_t step = henc->step;
    henc->step = 0;  // 读取后清零
    return step;
}


// 编码器初始化
void Drv_Encoder_Init(void)
{
    decoder_GPIO_Init();

    for (int i = 0; i < sizeof(encoder) / sizeof(encoder[0]); i++) {
        Encoder_Init(&encoder[i]);
    }

}

