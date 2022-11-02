TARGET = prog

CFLAGS = -std=c18 -Wall -Wextra -Wpedantic

.PHONY: all clean

all: $(TARGET)
clean:
	rm -f $(TARGET) *.o

$(TARGET): main.o test_getopt.o
	$(CC) -o $@ $^
