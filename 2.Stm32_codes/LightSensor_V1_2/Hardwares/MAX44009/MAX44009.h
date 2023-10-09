#ifndef __MAX44009_H__
#define __MAX44009_H__

#include "iic_hal.h"

#define INT_STATUS_REG				0x00
#define INT_ENABLE_REG              0x01
#define CONFIGURATION_REG           0x02
#define LUX_HIGH_REG                0x03
#define LUX_LOW_REG                 0x04
#define THRESHOLD_UPPER_REG         0x05
#define THRESHOLD_LOWER_REG         0x06
#define THRESHOLD_TIMER_REG         0x07

#define MAX44009_ADDR         		0x4A

// Integration time
#define MAX44009_IT_800ms ((uint8_t)0x00)  // 800ms
#define MAX44009_IT_400ms ((uint8_t)0x01)  // 400ms
#define MAX44009_IT_200ms ((uint8_t)0x02)  // 200ms
#define MAX44009_IT_100ms ((uint8_t)0x03)  // 100ms
#define MAX44009_IT_50ms ((uint8_t)0x04)   // 50ms
#define MAX44009_IT_25ms ((uint8_t)0x05)   // 25ms
#define MAX44009_IT_12d5ms ((uint8_t)0x06) // 12.5ms
#define MAX44009_IT_6d25ms ((uint8_t)0x07) // 6.25ms

uint8_t MAX_ReadOneReg(unsigned char RegAddr);
void MAX_WriteOneReg(unsigned char RegAddr, unsigned char dat);

uint8_t MAX44009_Init(void);
void MAX_Measure(float * Lux_value);

#endif
