#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <errno.h>
#include <zconf.h>
#include <sys/stat.h>

#include "../shared/client_msg_types.h"
#include "server.h"


int main() {
    server_t *server = init_server(malloc(sizeof(*server)), free);

    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1){
        while(waiting_messages(server->login_queue_id) > 0){
            client_msg msg;
            msgrcv(server->login_queue_id, &msg, sizeof(msg), 0, 0);
            switch (msg.msg_type){
                case LOGIN:
                    printf("Login request: id=%c, pid=%d\n", msg.payload.login_msg.client_id, msg.payload.login_msg.client_pid);
                    login_client(server, msg.payload.login_msg.client_id, msg.payload.login_msg.client_pid);
                    break;
                default:
                    printf("Malformed message %d\n", msg.msg_type);
                    break;
            }
        }
    }
    #pragma clang diagnostic pop


    free_server(server, free);
}