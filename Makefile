TARGET = prog
SRCS = main.c test_getopt.c util.c
OBJS = $(SRCS:.c=.o)

CFLAGS = -std=c18 -Wall -Wextra -Wpedantic
FORMATTER = clang-format-14 -i

.PHONY: all clean check format

all: $(TARGET)
clean:
	rm -f $(TARGET) *.o
check:
	./$(TARGET)
format:
	$(FORMATTER) $(SRCS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^
