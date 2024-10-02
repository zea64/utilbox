.PHONY: all
CC := gcc
CFLAGS := -O2 -Wall -Wextra

all: utilbox

utilbox: *.c *.h
	$(CC) $(CFLAGS) *.c -o $@
