#ifndef CONSUMER_H
#define CONSUMER_H
#include "buffer.h"

typedef struct {
    int id;
    SharedBuffer *buf;
    int rate_ms;
    volatile int *running;
} ConsumerArgs;

void *consumer_thread(void *arg);

#endif
