.PHONY: all clean
CC := gcc
CFLAGS := -O2 -Wall -Wextra

all: utilbox

clean:
	rm utilbox

utilbox: *.c *.h
	$(CC) $(CFLAGS) *.c -o $@
