#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "buffer.h"
#include "producer.h"
#include "consumer.h"
#include "logger.h"
#include "stats.h"

#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3
#define BUFFER_SIZE   10
#define GUI_REFRESH_MS 100

static volatile int running = 1;

void handle_signal(int sig) { running = 0; }

void draw_buffer(SharedBuffer *buf) {
    mvprintw(3, 2, "Buffer [%d/%d]: ", buf->count, buf->size);
    for (int i = 0; i < buf->size; i++) {
        int slot = (buf->out + i) % buf->size;
        if (i < buf->count) {
            attron(COLOR_PAIR(1));
            mvprintw(3, 18 + i * 5, "[%3d]", buf->items[slot]);
            attroff(COLOR_PAIR(1));
        } else {
            attron(COLOR_PAIR(2));
            mvprintw(3, 18 + i * 5, "[   ]");
            attroff(COLOR_PAIR(2));
        }
    }
}

void draw_stats(int start_row) {
    mvprintw(start_row, 2, "%-12s %-6s %-10s %-10s",
             "Thread", "Type", "Processed", "Sleeps(ms)");
    mvhline(start_row + 1, 2, '-', 50);
    for (int i = 0; i < NUM_PRODUCERS + NUM_CONSUMERS; i++) {
        ThreadStats s = stats_get(i);
        if (s.thread_id < 0) continue;
        double avg = s.items_processed > 0
                   ? s.total_sleep_ms / s.items_processed : 0;
        mvprintw(start_row + 2 + i, 2, "Thread-%-5d %-6s %-10ld %.1f ms avg",
                 s.thread_id, s.type, s.items_processed, avg);
    }
}

void draw_log(int start_row, int max_lines) {
    char lines[20][LOG_LINE_LEN];
    logger_get_recent(lines, max_lines);
    mvprintw(start_row, 2, "Activity log:");
    mvhline(start_row + 1, 2, '-', 76);
    for (int i = 0; i < max_lines; i++) {
        if (strlen(lines[i]) == 0) continue;
        // Color producers green, consumers blue
        if (strstr(lines[i], "PRODUCE")) attron(COLOR_PAIR(1));
        else attron(COLOR_PAIR(3));
        mvprintw(start_row + 2 + i, 2, "%-76s", lines[i]);
        attroff(COLOR_PAIR(1)); attroff(COLOR_PAIR(3));
    }
}

int main() {
    signal(SIGINT, handle_signal);
    srand(time(NULL));

    logger_init("simulation.log");
    stats_init();

    SharedBuffer buf;
    buffer_init(&buf, BUFFER_SIZE);

    // --- Create producer threads ---
    pthread_t prod_threads[NUM_PRODUCERS];
    ProducerArgs prod_args[NUM_PRODUCERS];
    int prod_rates[] = {300, 500, 800};  // variable production rates

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        prod_args[i] = (ProducerArgs){
            .id = i, .buf = &buf,
            .rate_ms = prod_rates[i],
            .items_to_make = 0,
            .running = &running
        };
        pthread_create(&prod_threads[i], NULL, producer_thread, &prod_args[i]);
    }

    // --- Create consumer threads ---
    pthread_t cons_threads[NUM_CONSUMERS];
    ConsumerArgs cons_args[NUM_CONSUMERS];
    int cons_rates[] = {400, 600, 700};

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        cons_args[i] = (ConsumerArgs){
            .id = NUM_PRODUCERS + i, .buf = &buf,
            .rate_ms = cons_rates[i],
            .running = &running
        };
        pthread_create(&cons_threads[i], NULL, consumer_thread, &cons_args[i]);
    }

    // --- ncurses setup ---
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);   
    init_pair(2, COLOR_RED,   COLOR_BLACK);   
    init_pair(3, COLOR_CYAN,  COLOR_BLACK);   

    while (running) {
        clear();
        box(stdscr, 0, 0);

        attron(A_BOLD);
        mvprintw(1, 2, "Multi-Threaded Producer-Consumer Simulator  [Ctrl+C to quit]");
        attroff(A_BOLD);

        draw_buffer(&buf);
        draw_stats(6);
        draw_log(6 + NUM_PRODUCERS + NUM_CONSUMERS + 4, 10);

        refresh();
        usleep(GUI_REFRESH_MS * 1000);
    }
    endwin();  
    for (int i = 0; i < NUM_PRODUCERS; i++) sem_post(&buf.full);
    for (int i = 0; i < NUM_CONSUMERS; i++) sem_post(&buf.empty);

    for (int i = 0; i < NUM_PRODUCERS; i++) pthread_join(prod_threads[i], NULL);
    for (int i = 0; i < NUM_CONSUMERS; i++) pthread_join(cons_threads[i], NULL);

    buffer_destroy(&buf);
    logger_destroy();

    printf("Simulation ended. Log saved to simulation.log\n");
    return 0;
}
