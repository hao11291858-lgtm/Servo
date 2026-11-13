#include "servo.h"
#include "tim.h"

#define SERVO_MIN_PULSE_US 1000U
#define SERVO_MAX_PULSE_US 2000U
#define SERVO_START_ANGLE  90U

HAL_StatusTypeDef Servo_Init(void)
{
    Servo_SetAngle(SERVO_START_ANGLE);
    return HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

void Servo_SetAngle(uint16_t angle)
{
    uint32_t pulse;

    if (angle > SERVO_MAX_ANGLE) {
        return;
    }

    /* TIM3 counts once per microsecond. */
    pulse = SERVO_MIN_PULSE_US +
            (uint32_t)angle * (SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US) / SERVO_MAX_ANGLE;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulse);
}
