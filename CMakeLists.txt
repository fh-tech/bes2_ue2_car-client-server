cmake_minimum_required(VERSION 3.10)
project(car_server_client)

set(CMAKE_C_STANDARD 11)

add_executable(display
        display/main.c)

add_executable(server
        server/main.c
        shared/client_msg_types.h
        server/server.h
        server/client.h
        server/server.c server/client.c)

add_executable(client
        client/main.c
        client/server.h
        client/server.c)