//
// Created by daniel on 01.05.18.
//

#ifndef CAR_SERVER_CLIENT_CLIENT_MSG_TYPES_H
#define CAR_SERVER_CLIENT_CLIENT_MSG_TYPES_H

#include <fcntl.h>

#define QUEUE_MSG_KEY 1
#define SERVER_LOGIN "/tmp/car-client-server_login"
#define SERVER_DISPLAY "/tmp/car-client-server_display"

#define SERVER_CLIENT_TO "/tmp/car-client-server_client_to_$"
#define SERVER_CLIENT_FROM "/tmp/car-client-server_client_from_$"

typedef struct {
    pid_t client_pid;
    char client_id;
} login_msg;

typedef struct {
    char action;
} client_action;

typedef struct {
    int x;
    int y;
} success_login;

enum msg_type {
    LOGIN,
    FAILED_LOGIN,
    SUCCESS_LOGIN,
    ACTION
};

typedef struct {
    enum msg_type msg_type;

    union {
        login_msg login_msg;
        client_action client_action;
        success_login success_login;
    } payload;

} client_msg;


#endif //CAR_SERVER_CLIENT_CLIENT_MSG_TYPES_H
