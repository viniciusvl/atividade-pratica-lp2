CC = gcc
CFLAGS = -O2 -Wall -Wextra
LIBS = -pthread -lm
TARGET = word_counter
SOURCES = src/main.c src/threads.c src/data.c src/create_input.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(shell find src -name "*.o")

.PHONY: all clean
