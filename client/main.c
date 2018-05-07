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


int main() {
    char id = 'B';
    server_t server = login(id, getpid());
    char cmd = '\0';
    while (cmd != 't') {
        scanf(" %c", &cmd);

        if(cmd != 't') {
            send_to_server(&server, cmd);
        }
    }
}
