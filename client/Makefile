CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -g
DEPS=. ../shared/*.h
OBS=main.o server.o

all: client

client: $(OBS)
	$(CC) $(CFLAGS) -I$(DEPS) $^ -o client

main.o: main.c $(DEPS)
server.o: server.c $(DEPS)

clean:
	rm *.o

.PHONY: all