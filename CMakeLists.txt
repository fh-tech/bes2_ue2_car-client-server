cmake_minimum_required(VERSION 3.9.6)
project(car_client_server2 C)

set(CMAKE_C_STANDARD 11)

add_executable(display
        display/main.c)

add_executable(server
        server/main.c
        server/server.c
        server/server.h
        server/client.h
        server/client.c)

include_directories(shared/)

add_library(messages
        shared/messages.h
        shared/messages.c)

add_executable(client
        client/main.c
        client/client.h
        client/client.c)

target_link_libraries(server messages)
target_link_libraries(display messages)
target_link_libraries(client messages)
