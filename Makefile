.PHONY: all clean
CC := clang
CFLAGS := -std=c23 -g -Os -Wall -Wextra

all: utilbox

clean:
	rm utilbox

utilbox: *.c *.h
	$(CC) $(CFLAGS) *.c -o $@
