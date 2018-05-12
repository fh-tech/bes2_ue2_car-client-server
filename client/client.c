//
// Created by daniel on 12.05.18.
//

#include <stdlib.h>
#include <messages.h>
#include <stdio.h>
#include <zconf.h>
#include <assert.h>

#include "client.h"


client_t *new_client(char id) {
    assert(id >= 'A' && id <= 'Z');

    client_t* client = malloc(sizeof(*client));
    client->to_server_mqid = -1;
    client->from_server_mqid = -1;
    client->id = id;
    return client;
}

void free_client(client_t* client) {
    free(client);
}

int try_login(client_t* client){

    int login_mqid = connect_mq(SERVER_LOGIN_QUEUE);
    if(login_mqid != -1){

        msg_wrap msg = {
                .msg_type=LOGIN,
                .buf.login.id=client->id,
                .buf.login.pid=getpid(),
        };

        send_to_mq(login_mqid, msg);

        //wait for server to setup queues
        usleep(100000);
        //connect to queues

        const char * from_client_str = get_from_client_str(client->id);
        const char * to_client_str = get_to_client_str(client->id);

        int to_server = connect_mq(from_client_str);
        int from_server = connect_mq(to_client_str);
        free((void*) from_client_str);
        free((void*) to_client_str);

        //if connection successful
        if(to_server != -1 && from_server != -1){
            //read login response from queue

            client->to_server_mqid = to_server;
            client->from_server_mqid = from_server;

            msg_wrap msg;
            if(receive_from_mq_block(from_server, &msg)){

                switch (msg.msg_type) {
                    case LOGIN_SUCCESS:
                        fprintf(stdout, "Registration OK. Start position: %d,%d\n", msg.buf.login_success.pos_x, msg.buf.login_success.pos_y);
                        return 1;

                    case LOGIN_FAILED:
                        fprintf(stdout, "Registration FAILED!\nClient with id: %c is already logged into this server!\n Please choose another id.\n", client->id);
                        return 0;
                    default:
                        fprintf(stderr, "Received unexpected message with id: %ld\n", msg.msg_type);
                        return 0;
                }
            } else {
                fprintf(stderr, "Unable to receive login response from server!\n");
                exit(1);
            }
        } else {
            perror("Unable to connect to client queues: ");
            fprintf(stderr, "Server has not created the queue for this client.\n");
            exit(1);
        }

    } else {
        perror("Unable to connect to login queue: ");
        fprintf(stderr, "Server process probably not started.\n");
        exit(1);
    }
}
