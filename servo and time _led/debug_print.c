#include "usart.h"
#include <stdio.h>

/* Blocking USART1 output after UART init; call from main, not interrupts. */
/* ARMCC5: use the UART instead of debugger semihosting. */
#pragma import(__use_no_semihosting)

struct __FILE {
    int error;
};

FILE __stdout;

int fputc(int ch, FILE *stream)
{
    uint8_t byte = (uint8_t)ch;

    if (HAL_UART_Transmit(&huart1, &byte, 1U, 100U) != HAL_OK) {
        stream->error = 1;
        return EOF;
    }
    return byte;
}

int ferror(FILE *stream)
{
    return stream->error;
}

void _ttywrch(int ch)
{
    (void)fputc(ch, stdout);
}

void _sys_exit(int status)
{
    (void)status;
    while (1) {
    }
}
