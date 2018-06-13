all: server client display

server:
	cd server && make server
	cd server && mv server ../build

client:
	cd client && make client
	cd client && mv client ../build

display:
	cd display && make display
	cd display && mv display ../build

clean:
	rm -f build/*

.PHONY: server client display clean

