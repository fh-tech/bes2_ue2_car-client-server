CC=gcc
CFLAGS=-Wall -Wextra -g
INCLUDE= -I../shared
DEPS= client.h server.h ../shared/messages.h

server: main.o client.o server.o ../build/messagelib.a
	$(CC) $^ -o server

main.o: main.c $(DEPS)
	$(CC) -c $^ $(INCLUDE)

client.o: client.c $(DEPS)
	$(CC) -c $^ $(INCLUDE)

server.o: server.c $(DEPS)
	$(CC) -c $^ $(INCLUDE)


../build/messagelib.a: ../shared/*
	cd ../shared/ && make all && make clean
	cd ../shared/ && mv messagelib.a ../build

clean:
	rm -f *.o *.gch

.PHONY: server clean
