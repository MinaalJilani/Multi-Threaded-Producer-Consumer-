#ifndef STATS_H
#define STATS_H
#include <pthread.h>

#define MAX_THREADS 16

typedef struct {
    int thread_id;
    char type[16];          
    long items_processed;
    double total_sleep_ms;
    int wait_count;         
} ThreadStats;

void stats_init();
void stats_record(int thread_id, const char *type, double sleep_ms, int waited);
ThreadStats stats_get(int thread_id);
void stats_print_all();

#endif
