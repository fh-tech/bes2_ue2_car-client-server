//
// Created by daniel on 11.05.18.
//

#include "client.h"

void free_client(client *c) {
    msgctl(c->to_client_msg_q, IPC_RMID, NULL);
    msgctl(c->from_client_msg_q, IPC_RMID, NULL);
    kill(c->pid, 15);
    free(c);
}
