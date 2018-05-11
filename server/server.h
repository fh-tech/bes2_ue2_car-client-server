//
// Created by daniel on 02.05.18.
//

#ifndef CAR_SERVER_CLIENT_SERVER_H
#define CAR_SERVER_CLIENT_SERVER_H

#include <sys/types.h>
#include <stdlib.h>
#include <sys/msg.h>
#include "client.h"
#include "../shared/client_msg_types.h"



typedef struct server_s {

    client *clients[26];
    u_int8_t last_client;

    int login_queue_id;

    int width;
    int height;

} server_t;


server_t* init_server(server_t *server, void (*free)(void *));

void free_server(server_t* server, void(*free)(void*));

int send_to_client(server_t* server, client_msg msg);

const char* display_state(server_t *server);

void login_client(server_t* server, char id, pid_t pid);

msgqnum_t waiting_messages(int mesg_id);

#endif //CAR_SERVER_CLIENT_SERVER_H
