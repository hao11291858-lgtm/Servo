#include "usart_transport.h"
#include "fifo.h"
#include "usart.h"

#define RX_FIFO_SIZE 64U
#define RX_IDLE_MS   100U

static Fifo rx_fifo;
static uint8_t rx_storage[RX_FIFO_SIZE];
static uint8_t rx_byte;
static volatile uint32_t last_rx_tick;
static volatile bool rx_active;
static volatile bool rx_lost;

HAL_StatusTypeDef UsartTransport_Init(void)
{
    Fifo_Init(&rx_fifo, rx_storage, sizeof(rx_storage));
    rx_active = false;
    rx_lost = false;
    return HAL_UART_Receive_IT(&huart1, &rx_byte, 1U);
}

UsartTransportEvent UsartTransport_Read(uint8_t *byte)
{
    UsartTransportEvent event = USART_TRANSPORT_NO_DATA;
    uint32_t interrupt_state = __get_PRIMASK();

    /* The USART interrupt also accesses the FIFO and receive flags. */
    __disable_irq();
    if (rx_lost)
    {
        Fifo_Clear(&rx_fifo);
        rx_lost = false;
        event = USART_TRANSPORT_RX_ERROR;
    }
    else if (Fifo_Pop(&rx_fifo, byte))
    {
        event = USART_TRANSPORT_BYTE;
    }
    else if (rx_active && (HAL_GetTick() - last_rx_tick >= RX_IDLE_MS))
    {
        rx_active = false;
        event = USART_TRANSPORT_IDLE;
    }
    __set_PRIMASK(interrupt_state);

    return event;
}

HAL_StatusTypeDef UsartTransport_Send(const uint8_t *data, uint16_t size,
                                     uint32_t timeout_ms)
{
    return HAL_UART_Transmit(&huart1, data, size, timeout_ms);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *uart)
{
    /* HAL can report a byte and an error in the same interrupt. */
    if (uart != &huart1 || uart->ErrorCode != HAL_UART_ERROR_NONE)
    {
        return;
    }

    last_rx_tick = HAL_GetTick();
    rx_active = true;
    if (!Fifo_Push(&rx_fifo, rx_byte))
    {
        rx_lost = true;
    }

    HAL_UART_Receive_IT(uart, &rx_byte, 1U);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *uart)
{
    if (uart != &huart1)
    {
        return;
    }

    last_rx_tick = HAL_GetTick();
    rx_active = true;
    rx_lost = true;

    /* USART1 uses interrupt reception only. Restart it after the error. */
    HAL_UART_AbortReceive(uart);
    __HAL_UART_CLEAR_OREFLAG(uart);
    HAL_UART_Receive_IT(uart, &rx_byte, 1U);
}
