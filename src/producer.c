#include "producer.h"
#include "stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void *producer_thread(void *arg) {
    ProducerArgs *a = (ProducerArgs *)arg;
    int item_counter = 0;

    while (*a->running) {
        if (a->items_to_make > 0 && item_counter >= a->items_to_make) break;

        int item = (rand() % 900) + 100;
        buffer_produce(a->buf, item, a->id);
        item_counter++;
        stats_record(a->id, "PRODUCER", a->rate_ms, 0);
        usleep(a->rate_ms * 1000);
    }
    return NULL;
}
