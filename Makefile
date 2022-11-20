TARGET = prog
SRCS = main.c test_getopt.c test_getopt_long.c util.c
OBJS = $(SRCS:.c=.o)

CFLAGS = -std=c18 -g -Wall -Wextra -Wpedantic

ifeq ($(COVERAGE_TEST), 1)
	CFLAGS += -O0 -fprofile-arcs -ftest-coverage
	LIBS += -lgcov
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -O0 -g
endif

FORMATTER = clang-format-14 -i

.PHONY: all clean check gcov format

all: $(TARGET)
check: all
	./$(TARGET)
gcov:
	gcov $(SRCS)
clean:
	rm -f $(TARGET) *.o *.gcda *.gcno *.gcov
gen_db:
	./gen_cmpl_db.sh
format:
	$(FORMATTER) $(SRCS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

main.o: main.h
