//
// Created by daniel on 12.05.18.
//

#ifndef CAR_CLIENT_SERVER2_CLIENT_H
#define CAR_CLIENT_SERVER2_CLIENT_H

typedef struct client_s
{
    char id;
    int to_server_mqid;
    int from_server_mqid;
}
client_t;

client_t* new_client(char id);

int try_login(client_t* client);

void free_client(client_t* client);

#endif //CAR_CLIENT_SERVER2_CLIENT_H
