#include "buffer.h"
#include "logger.h"
#include <stdio.h>

void buffer_init(SharedBuffer *buf, int size) {
    buf->in = 0;
    buf->out = 0;
    buf->count = 0;
    buf->size = size;

    pthread_mutex_init(&buf->mutex, NULL);
    sem_init(&buf->empty, 0, size);  
    sem_init(&buf->full, 0, 0);      
}

bool buffer_produce(SharedBuffer *buf, int item, int producer_id) {

    sem_wait(&buf->empty);
    pthread_mutex_lock(&buf->mutex);

    buf->items[buf->in] = item;
    buf->in = (buf->in + 1) % buf->size;
    buf->count++;
    
    pthread_mutex_unlock(&buf->mutex);
    sem_post(&buf->full);

    log_event("PRODUCE", producer_id, item, buf->count, buf->size);
    return true;
}
bool buffer_consume(SharedBuffer *buf, int *item, int consumer_id) {
    
    sem_wait(&buf->full);

    pthread_mutex_lock(&buf->mutex);

    *item = buf->items[buf->out];
    buf->out = (buf->out + 1) % buf->size;
    buf->count--;

    pthread_mutex_unlock(&buf->mutex);

    sem_post(&buf->empty);

    log_event("CONSUME", consumer_id, *item, buf->count, buf->size);
    return true;
}

void buffer_destroy(SharedBuffer *buf) {
    pthread_mutex_destroy(&buf->mutex);
    sem_destroy(&buf->empty);
    sem_destroy(&buf->full);
}
