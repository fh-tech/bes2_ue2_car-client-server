//
// Created by daniel on 05.05.18.
//

#ifndef CAR_SERVER_CLIENT_SERVER_H
#define CAR_SERVER_CLIENT_SERVER_H

#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include "../shared/client_msg_types.h"

#define SERVER_LOGIN "/tmp/car-client-server_login"

typedef struct {
    int to_server_msg_id;
    int from_server_msg_id;
} server_t;



server_t login(char id, pid_t pid);


int try_connect(char* unique_str);

#endif //CAR_SERVER_CLIENT_SERVER_H
