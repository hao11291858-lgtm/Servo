#include "fifo.h"

void Fifo_Init(Fifo *fifo, uint8_t *storage, uint16_t capacity)
{
    fifo->storage = storage;
    fifo->capacity = capacity;
    Fifo_Clear(fifo);
}

void Fifo_Clear(Fifo *fifo)
{
    fifo->read_index = 0;
    fifo->write_index = 0;
    fifo->count = 0;
}

bool Fifo_Push(Fifo *fifo, uint8_t value)
{
    if (fifo->count == fifo->capacity)
    {
        return false;
    }

    fifo->storage[fifo->write_index] = value;
    fifo->write_index++;
    if (fifo->write_index == fifo->capacity)
    {
        fifo->write_index = 0;
    }
    fifo->count++;
    return true;
}

bool Fifo_Pop(Fifo *fifo, uint8_t *value)
{
    if (fifo->count == 0)
    {
        return false;
    }

    *value = fifo->storage[fifo->read_index];
    fifo->read_index++;
    if (fifo->read_index == fifo->capacity)
    {
        fifo->read_index = 0;
    }
    fifo->count--;
    return true;
}
