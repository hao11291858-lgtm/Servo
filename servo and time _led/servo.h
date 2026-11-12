#ifndef SERVO_H
#define SERVO_H
#include "main.h"

/* Poll one raw USART1 byte (0x00-0xB4) and apply it immediately.
 * 0-180 maps to 1000-2000 us; 0x5A is the nominal continuous-servo stop (1500 us).
 * Use HEX send with no CR/LF suffix: 0x0A and 0x0D are also valid commands.
 * Out-of-range or damaged bytes leave the current PWM pulse unchanged. */
void servo_run(void);

#endif
