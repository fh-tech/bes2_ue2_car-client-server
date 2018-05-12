//
// Created by daniel on 11.05.18.
//

#ifndef CAR_CLIENT_SERVER2_CLIENT_H
#define CAR_CLIENT_SERVER2_CLIENT_H

#include <sys/types.h>
#include <signal.h>
#include <memory.h>
#include "messages.h"

typedef struct client_s
{
    pid_t pid;
    char id;

    int to_client_mqid;
    int from_client_mqid;

    int pos_x;
    int pos_y;
}
client_t;

void free_client(client_t* client);

client_t* new_client(pid_t pid, char id, int x, int y);




#endif //CAR_CLIENT_SERVER2_CLIENT_H
