#ifndef __HXZP_DECODER_H__
#define __HXZP_DECODER_H__

#include <stdint.h>
#include "stm32f1xx_hal.h"  // 根据实际MCU修改

// 编码器方向定义
typedef enum {
    ENCODER_DIR_NONE = 0,    // 无方向/停止状态
    ENCODER_DIR_CW,          // 顺时针方向
    ENCODER_DIR_CCW          // 逆时针方向
} EncoderDirection;

// 编码器状态结构体（扩展版）
typedef struct {
    GPIO_TypeDef* GPIOA_Port;
    uint16_t GPIOA_Pin;
    GPIO_TypeDef* GPIOB_Port;
    uint16_t GPIOB_Pin;
    
    uint8_t stateA;
    uint8_t stateB;
    uint8_t last_state;
    int32_t counter;
    int8_t step;
    
    // 新增防抖相关字段
    EncoderDirection confirmed_dir;   // 已确认的方向
    EncoderDirection candidate_dir;   // 候选方向
    uint8_t debounce_count;           // 防抖计数器
    uint8_t debounce_threshold;       // 防抖阈值
    uint32_t last_change_time;        // 最后变化时间
} Encoder_HandleTypeDef;

void Drv_Encoder_Init(void);

void Encoder_PollAll(void);

int8_t Encoder_GetStep(uint8_t enc_index);

void Encoder_ResetCounter(uint8_t enc_index);

void Encoder_ResetCounterAll(void);

int32_t Encoder_GetCounter(uint8_t enc_index);

EncoderDirection Encoder_GetDirection(uint8_t enc_index);
#endif





