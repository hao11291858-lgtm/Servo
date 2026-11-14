#include "servo.h"
#include "usart.h"
#include "tim.h"
void servo_run(void)
{
    uint8_t ch;         // 保存串口收到的一个字符
    static uint8_t angle = 0; // 保存输入的角度
    // 等待串口收到一个字符
    if (HAL_UART_Receive(&huart1, &ch, 1, HAL_MAX_DELAY) == HAL_OK)
    {
        HAL_UART_Transmit(&huart1,&ch, 1,100);
        // 收到数字，拼成角度
        if (ch >= '0' && ch <= '9')
        {
            angle = angle * 10 + (ch - '0');
            // 最大限制为180°
            if (angle > 180)
            {
                angle = 180;
            }
        }
        // 收到换行，控制舵机
        else if (ch == '\n')
        {           
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 500 + angle * 2000 / 180);
            angle = 0; // 准备接收下一个角度
        }
    }
}
