CC = gcc
CFLAGS = -W -Wall -O2
SRCS = main.c communicate.c error.c
OBJS = main.o communicate.o error.o
TARGET = socket_daemon

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

main.o: main.c communicate.h error.h
	$(CC) $(CFLAGS) -c $<

communicate.o: communicate.c communicate.h error.h
	$(CC) $(CFLAGS) -c $<

error.o: error.c error.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJS)
