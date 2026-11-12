#include "servo.h"
#include "usart.h"
#include "tim.h"

#define SERVO_MAX_COMMAND  180U
#define SERVO_MIN_PULSE_US 1000U
#define SERVO_MAX_PULSE_US 2000U

void servo_run(void)
{
    uint8_t command;
    uint32_t uart_flags = huart1.Instance->SR;

    if ((uart_flags & (UART_FLAG_ORE | UART_FLAG_NE | UART_FLAG_FE | UART_FLAG_PE)) != 0U) {
        /* On STM32F1, reading SR then DR clears all four receive errors.
         * Discard the damaged byte; the next byte is a new command. */
        __HAL_UART_CLEAR_OREFLAG(&huart1);
        return;
    }

    if ((uart_flags & UART_FLAG_RXNE) == 0U) {
        return;
    }

    /* Poll one byte without holding up the main loop when no data is ready. */
    if (HAL_UART_Receive(&huart1, &command, 1U, 0U) != HAL_OK) {
        return;
    }

    if (command > SERVO_MAX_COMMAND) {
        return;
    }

    /* TIM3 counts at 1 MHz. Apply each raw byte immediately, without CR/LF. */
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,
        SERVO_MIN_PULSE_US
        + (uint32_t)command * (SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US)
            / SERVO_MAX_COMMAND);
}
