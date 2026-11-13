#include "usart_transport.h"
#include "fifo.h"
#include "usart.h"

#define RX_FIFO_SIZE 64U

static Fifo    rx_fifo;
static uint8_t rx_storage[RX_FIFO_SIZE];

void UsartTransport_Init(void)
{
    Fifo_Init(&rx_fifo, rx_storage, sizeof(rx_storage));
    __HAL_UART_CLEAR_OREFLAG(&huart1);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);
}

bool UsartTransport_Read(uint8_t *byte)
{
    bool     received;
    uint32_t interrupt_state = __get_PRIMASK();

    /* Serialize the FIFO pop with the interrupt-side push. */
    __disable_irq();
    received = Fifo_Pop(&rx_fifo, byte);
    __set_PRIMASK(interrupt_state);

    return received;
}

void UsartTransport_IRQHandler(void)
{
    uint32_t status = READ_REG(huart1.Instance->SR);

    if ((status & (USART_SR_PE | USART_SR_FE | USART_SR_NE | USART_SR_ORE)) != 0U) {
        /* Reading SR then DR clears receive errors; discard the bad byte. */
        (void)READ_REG(huart1.Instance->DR);
        return;
    }

    if ((status & USART_SR_RXNE) != 0U) {
        /* A full FIFO drops the new byte; RXNE interrupts remain enabled. */
        Fifo_Push(&rx_fifo, (uint8_t)READ_REG(huart1.Instance->DR));
    }
}
