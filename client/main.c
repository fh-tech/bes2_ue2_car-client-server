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


int main(){
    char id = 'A';

    perror("");

    server_t server = login(id, getpid());


    perror("");
}
