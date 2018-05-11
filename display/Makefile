CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -g
DEPS=. ../shared/*
OBS=main.o

all: display

display: $(OBS)
	$(CC) $(CFLAGS) -I$(DEPS) $^ -o display

main.o: main.c $(DEPS)

clean:
	rm *.o

.PHONY: all