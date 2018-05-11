CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -g -Wno-unused-variable -Wno-unused-parameter
DEPS=. ../shared/*.h
OBS=main.o server.o client.o

all: server

server: $(OBS)
	$(CC) $(CFLAGS) -I$(DEPS) $^ -o server

main.o: main.c $(DEPS)
server.o: server.c $(DEPS)
client.o: client.c $(DEPS)

clean:
	rm *.o

.PHONY: all