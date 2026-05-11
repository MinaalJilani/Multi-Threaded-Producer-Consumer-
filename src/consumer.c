#include "consumer.h"
#include "stats.h"
#include <unistd.h>
#include <stdlib.h>

void *consumer_thread(void *arg) {
    ConsumerArgs *a = (ConsumerArgs *)arg;

    while (*a->running) {
        int item;
        buffer_consume(a->buf, &item, a->id);
        stats_record(a->id, "CONSUMER", a->rate_ms, 0);
        usleep(a->rate_ms * 1000);
    }
    return NULL;
}
