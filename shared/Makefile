
all: sharedlib

sharedlib: *.o
	ar rcs messagelib.a *.o

*.o: *.c *.h
	gcc -c *.c

clean:
	rm -f *.o *.gch

.PHONY: clean all