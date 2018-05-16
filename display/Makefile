CC=gcc
CFLAGS=-Wall -Wextra -g
INCLUDE= -I../shared
DEPS=../shared/messages.h

display: main.o ../build/messagelib.a
	$(CC) $^ -o display

main.o: main.c $(DEPS)
	$(CC) -c $^ $(INCLUDE)

../build/messagelib.a: ../shared/*
	cd ../shared/ && make all && make clean
	cd ../shared/ && mv messagelib.a ../build

clean:
	rm -f *.o *.gch

.PHONY: display clean
