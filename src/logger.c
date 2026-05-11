#include "logger.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

static FILE *log_file = NULL;
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

static char log_ring[MAX_LOG_LINES][LOG_LINE_LEN];
static int log_head = 0;  
static int log_total = 0;

void logger_init(const char *filename) {
    log_file = fopen(filename, "w");
}

void log_event(const char *action, int thread_id, int item,
               int buf_count, int buf_size) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long ms = ts.tv_nsec / 1000000;

    char line[LOG_LINE_LEN];
    snprintf(line, sizeof(line),
             "[%ld.%03ld] %-8s Thread-%d  item=%-4d  buffer=%d/%d",
             ts.tv_sec % 1000, ms, action, thread_id, item, buf_count, buf_size);

    pthread_mutex_lock(&log_mutex);
    strncpy(log_ring[log_head], line, LOG_LINE_LEN - 1);
    log_head = (log_head + 1) % MAX_LOG_LINES;
    log_total++;
    if (log_file) {
        fprintf(log_file, "%s\n", line);
        fflush(log_file);
    }
    pthread_mutex_unlock(&log_mutex);
}

void logger_get_recent(char lines[][LOG_LINE_LEN], int n) {
    pthread_mutex_lock(&log_mutex);
    int total = log_total < MAX_LOG_LINES ? log_total : MAX_LOG_LINES;
    int count = n < total ? n : total;
    for (int i = 0; i < count; i++) {
        int idx = (log_head - count + i + MAX_LOG_LINES) % MAX_LOG_LINES;
        strncpy(lines[i], log_ring[idx], LOG_LINE_LEN - 1);
    }
    pthread_mutex_unlock(&log_mutex);
}

void logger_destroy() {
    if (log_file) fclose(log_file);
}
