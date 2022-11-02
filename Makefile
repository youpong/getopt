TARGET = prog
SRCS = main.c test_getopt.c util.c
OBJS = $(SRCS:.c=.o)

CFLAGS = -std=c18 -Wall -Wextra -Wpedantic

.PHONY: all clean check

all: $(TARGET)
clean:
	rm -f $(TARGET) *.o
check:
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^
