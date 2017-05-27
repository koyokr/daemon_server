CC = gcc
CFLAGS = -W -Wall -O2
SRCS = main.c play.c error.c
OBJS = main.o play.o error.o
TARGET = server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

main.o: main.c error.h
	$(CC) $(CFLAGS) -O2 -c $<

play.o: play.c error.h
	$(CC) $(CFLAGS) -O2 -c $<

error.o: error.c error.h
	$(CC) $(CFLAGS) -O2 -c $<

clean:
	rm -f $(TARGET) $(OBJS)
