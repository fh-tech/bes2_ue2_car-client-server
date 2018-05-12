all: server client display

server:
	cd server && make server && make clean
	cd server && mv server ../build

client:
	cd client && make client && make clean
	cd client && mv client ../build

display:
	cd display && make display && make clean
	cd display && mv display ../build

clean:
	rm -f build/*

.PHONY: server client display clean

