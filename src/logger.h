#ifndef LOGGER_H
#define LOGGER_H

#define MAX_LOG_LINES 200
#define LOG_LINE_LEN  120

void logger_init(const char *filename);
void log_event(const char *action, int thread_id, int item, int buf_count, int buf_size);
void logger_get_recent(char lines[][LOG_LINE_LEN], int n);
void logger_destroy();

#endif
