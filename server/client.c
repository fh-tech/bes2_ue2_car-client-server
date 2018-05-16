//
// Created by daniel on 11.05.18.
//
#include <zconf.h>
#include "client.h"

void free_client(client_t *client) {

    if(client->to_client_mqid){
        delete_mq(client->to_client_mqid);
        const char* to_client_pipe = get_to_client_str(client->id);
        unlink(to_client_pipe);
        free((void*)to_client_pipe);
    }

    if(client->from_client_mqid){
        delete_mq(client->from_client_mqid);
        const char* from_client_pipe = get_from_client_str(client->id);
        unlink(from_client_pipe);
        free((void*)from_client_pipe);
    }

    kill(client->pid, 15);
    free(client);
}



client_t *new_client(pid_t pid, char id, int x, int y) {

    client_t* client = malloc(sizeof(*client));

    const char* to_client = get_to_client_str(id);
    const char* from_client = get_from_client_str(id);

    int to_client_mq = create_mq(to_client);
    int from_client_mq = create_mq(from_client);

    free((void *) to_client);
    free((void *) from_client);

    client->id = id;
    client->pid = pid;

    client->to_client_mqid = to_client_mq;
    client->from_client_mqid = from_client_mq;
    client->pos_x = x;
    client->pos_y = y;

    if(to_client_mq != -1 && from_client_mq != -1){
        return client;
    }else{
        free_client(client);
        return NULL;
    }
}
