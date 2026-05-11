#include "stats.h"
#include <string.h>
#include <stdio.h>

static ThreadStats all_stats[MAX_THREADS];
static pthread_mutex_t stats_mutex = PTHREAD_MUTEX_INITIALIZER;

void stats_init() {
    memset(all_stats, 0, sizeof(all_stats));
    for (int i = 0; i < MAX_THREADS; i++) all_stats[i].thread_id = -1;
}

void stats_record(int thread_id, const char *type, double sleep_ms, int waited) {
    if (thread_id < 0 || thread_id >= MAX_THREADS) return;
    pthread_mutex_lock(&stats_mutex);
    all_stats[thread_id].thread_id = thread_id;
    strncpy(all_stats[thread_id].type, type, 15);
    all_stats[thread_id].items_processed++;
    all_stats[thread_id].total_sleep_ms += sleep_ms;
    all_stats[thread_id].wait_count += waited;
    pthread_mutex_unlock(&stats_mutex);
}

ThreadStats stats_get(int thread_id) {
    return all_stats[thread_id];
}
