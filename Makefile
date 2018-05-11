BUILD_DIR=build

all: server client display

server:
	cd ./server && make -b all && make clean
	mv server/server ./$(BUILD_DIR)

client:
	cd ./client && make -b all && make clean
	mv client/client ./$(BUILD_DIR)

display:
	cd ./display && make -b all && make clean
	mv display/display ./$(BUILD_DIR)

.PHONY: server client display