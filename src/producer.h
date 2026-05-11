#ifndef PRODUCER_H
#define PRODUCER_H
#include "buffer.h"

typedef struct {
    int id;
    SharedBuffer *buf;
    int rate_ms;         
    int items_to_make;   
    volatile int *running;
} ProducerArgs;

void *producer_thread(void *arg);

#endif
