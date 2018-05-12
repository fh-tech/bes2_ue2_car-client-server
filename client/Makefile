CC=gcc
CFLAGS=-Wall -Wextra -g
INCLUDE= -I../shared
DEPS= client.h ../shared/messages.h

client: main.o client.o ../build/messagelib.a
	$(CC) $^ -o client

main.o: main.c $(DEPS)
	$(CC) -c $^ $(INCLUDE)

client.o: client.c $(DEPS)
	$(CC) -c $^ $(INCLUDE)

../build/messagelib.a: ../shared/*
	cd ../shared/ && make all && make clean
	cd ../shared/ && mv messagelib.a ../build

clean:
	rm -f *.o *.gch

.PHONY: client clean
