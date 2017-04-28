
SRCS = main.c play.c error.c
TARGET = server

all: $(TARGET)

$(TARGET): $(SRCS) error.h
	$(CC) -O2 -o $@ $(SRCS)

clean:
	rm $(TARGET)
