#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <errno.h>
#include <zconf.h>
#include <sys/stat.h>

#include "../shared/client_msg_types.h"
#include "server.h"

int run = 1;

void sig_handle(int signal){
    printf("Requesting shutdown with signal: %d\n", signal);
    run = 0;
}


int main() {

    signal(15, sig_handle);
    signal(9, sig_handle);

    server_t *server = init_server(malloc(sizeof(*server)), free);
    while (run){
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
        usleep(10000);
    }
    printf("Shutting down...\n");
    free_server(server, free);
    return 0;
}