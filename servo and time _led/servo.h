#ifndef SERVO_H
#define SERVO_H

#include "stm32f1xx_hal.h"

#define SERVO_MAX_ANGLE 180U

HAL_StatusTypeDef Servo_Init(void);
void              Servo_SetAngle(uint16_t angle);

#endif
