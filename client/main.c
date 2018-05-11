//
// Created by daniel on 05.05.18.
//
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <zconf.h>
#include <stdio.h>

#include "../shared/client_msg_types.h"
#include "server.h"


int main(int argc, char** argv) {
    char id = '\0';

    if(argc == 2){
        id = *argv[1];
        printf("Creating client with id: %c\n", id);

        server_t server = login(id, getpid());
        char cmd = '\0';
        while (cmd != 't') {
            scanf(" %c", &cmd);

            if(cmd != 't') {
                send_to_server(&server, cmd);
            }
        }
    }


}
