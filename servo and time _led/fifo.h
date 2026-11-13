#ifndef FIFO_H
#define FIFO_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t *storage;
    uint16_t capacity;
    uint16_t read_index;
    uint16_t write_index;
    uint16_t count;
} Fifo;

void Fifo_Init(Fifo *fifo, uint8_t *storage, uint16_t capacity);
void Fifo_Clear(Fifo *fifo);
bool Fifo_Push(Fifo *fifo, uint8_t value);
bool Fifo_Pop(Fifo *fifo, uint8_t *value);

#endif
