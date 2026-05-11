CC = gcc
CFLAGS = -Wall -Wextra -pthread -g
LIBS = -lncurses -lpthread

SRC = src/main.c src/buffer.c src/producer.c src/consumer.c \
      src/logger.c src/stats.c

TARGET = simulator

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET) simulation.log

run: all
	./simulator
