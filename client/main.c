//
// Created by daniel on 11.05.18.
//
#include <zconf.h>
#include <messages.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include "../shared/messages.h"
#include "client.h"


static client_t* client = NULL;

void signal_handler(int signal) {
    free_client(client);
    exit(0);
}

int main(int argc, char **argv) {
    srand(time(NULL));

    signal(15, signal_handler);
    signal(2, signal_handler);

    // client id first program arg or A if not specified
    char id = (random()%26) + 'A';
    if (argc > 1) {
        id = *argv[1];
    }

    client = new_client(id);

    //login to server and initialize message queues
    //also checks for successful server response

    if (try_login(client)) {

        char cmd = '\0';
        while (cmd != 'T') {
            //read cmd to send to server
            scanf(" %c", &cmd);
            // termination request
            if (cmd == 'T'){

                msg_wrap msg = {
                        .msg_type=LOGOUT,
                        .buf.empty={}
                };
                send_to_mq(client->to_server_mqid, msg);
            }
            //move car in dir
            if  (  cmd == 'N'
                || cmd == 'E'
                || cmd == 'S'
                || cmd == 'W'
            ) {
                //***send cmd to server***
                msg_wrap msg = {
                        .msg_type=CLIENT_ACTION,
                        .buf.client_action.action=cmd,
                };
                send_to_mq(client->to_server_mqid, msg);
                //********************
            } else {
                printf("Not a valid command!\n Type N, E, S, W to move your car in the corresponding direction or T to end this session.\n");
            }
        }
    }
    free_client(client);
}