TARGET = prog
SRCS = main.c test_getopt.c util.c
OBJS = $(SRCS:.c=.o)

CFLAGS = -std=c18 -Wall -Wextra -Wpedantic -g
FORMATTER = clang-format-14 -i

.PHONY: all clean check format

all: $(TARGET)
check: all
	./$(TARGET)
clean:
	rm -f $(TARGET) *.o
gen_db:
	./gen_cmpl_db.sh
format:
	$(FORMATTER) $(SRCS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

main.o: main.h
