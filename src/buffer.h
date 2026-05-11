#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

#define MAX_BUFFER_SIZE 20

typedef struct {
    int items[MAX_BUFFER_SIZE];
    int in;          
    int out;         
    int count;       
    int size;        

    pthread_mutex_t mutex;   
    sem_t empty;             
    sem_t full;             
} SharedBuffer;

void buffer_init(SharedBuffer *buf, int size);
bool buffer_produce(SharedBuffer *buf, int item, int producer_id);
bool buffer_consume(SharedBuffer *buf, int *item, int consumer_id);
void buffer_destroy(SharedBuffer *buf);

#endif
