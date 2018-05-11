//
// Created by daniel on 02.05.18.
//

#ifndef CAR_SERVER_CLIENT_CLIENT_H
#define CAR_SERVER_CLIENT_CLIENT_H

#include <stdlib.h>
#include <signal.h>
#include <sys/msg.h>

typedef struct {
    char id;
    pid_t pid;
    int to_client_msg_q;
    int from_client_msg_q;

    enum DIR {
        N, E, S, W
    } dir;

    int x;
    int y;
} client;

void free_client(client* c);


#endif //CAR_SERVER_CLIENT_CLIENT_H
