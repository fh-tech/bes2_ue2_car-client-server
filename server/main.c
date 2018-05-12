//
// Created by daniel on 11.05.18.
//

#include <stdio.h>
#include <signal.h>
#include <zconf.h>
#include <time.h>
#include "../shared/messages.h"
#include "server.h"

static int run = 1;

void signal_handler(int sig){
    printf("Requesting shutdown with signal: %d", sig);
    run = 0;
}

int main(){
    srand(time(NULL));
    signal(15, signal_handler);
    signal(2, signal_handler);

    int width = 10, height = 10;

    server_t* server = new_server(width, height);

    while (run){
        handle_login(server);
        handle_clients(server);

        usleep(100000);
    }


    free_server(server);
}
