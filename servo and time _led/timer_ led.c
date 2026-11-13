#include "timer_led.h"
#include "tim.h"
#include "gpio.h"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    }
}
