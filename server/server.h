//
// Created by daniel on 11.05.18.
//

#ifndef CAR_CLIENT_SERVER2_SERVER_H
#define CAR_CLIENT_SERVER2_SERVER_H

#include <messages.h>
#include "client.h"

#define MAX_CLIENTS 26

typedef struct server_s {
    int login_mqid;
    int display_pipe_fd;

    int width;
    int height;

    char* field;

    client_t *clients[MAX_CLIENTS];

} server_t;

typedef struct{
    int x, y;
} pos;

pos generate_new_pos(server_t* server);

char get_client_at_pos(server_t* server, int x, int y);

void set_client_at_pos(server_t* server, int x, int y, client_t* client);

void set_at_pos(server_t *server, int x, int y, char symbol);

server_t *new_server(int width, int height);

void free_server(server_t *server);

void handle_login(server_t *server);

void logout(server_t* server, client_t* client);

void client_action(server_t *server, client_t* client, char action);

void handle_client(server_t* server, client_t* client);

void handle_clients(server_t* server);

void set_client(server_t *server, client_t *client);

client_t *get_client(server_t *server, char id);

void update_display(server_t* server);

#endif //CAR_CLIENT_SERVER2_SERVER_H
