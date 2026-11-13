#ifndef USART_TRANSPORT_H
#define USART_TRANSPORT_H

#include "stm32f1xx_hal.h"

typedef enum
{
    USART_TRANSPORT_NO_DATA,
    USART_TRANSPORT_BYTE,
    USART_TRANSPORT_IDLE,
    USART_TRANSPORT_RX_ERROR
} UsartTransportEvent;

/* Start once after MX_USART1_UART_Init(). */
HAL_StatusTypeDef UsartTransport_Init(void);

/* Call from the main loop. IDLE is reported once after 100 ms of silence. */
UsartTransportEvent UsartTransport_Read(uint8_t *byte);

/* Optional blocking transmit for callers in the main loop. */
HAL_StatusTypeDef UsartTransport_Send(const uint8_t *data, uint16_t size,
                                     uint32_t timeout_ms);

#endif
