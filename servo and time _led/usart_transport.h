#ifndef USART_TRANSPORT_H
#define USART_TRANSPORT_H

#include <stdbool.h>
#include <stdint.h>

void UsartTransport_Init(void);
void UsartTransport_IRQHandler(void);
bool UsartTransport_Read(uint8_t *byte);

#endif
