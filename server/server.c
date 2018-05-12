//
// Created by daniel on 11.05.18.
//

#include <libzvbi.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include <zconf.h>

#include "server.h"
#include "messages.h"

client_t *get_client(server_t* server, char id) {
    assert(id >= 'A' && id <= 'Z');
    return server->clients[id-'A'];
}

server_t *new_server(int width, int height) {
    assert(width > 0 && height > 0);
    server_t* server = calloc(1, sizeof(*server));

    server->height = height;
    server->width  = width;

    server->login_mqid = -1;
    server->display_pipe_fd = -1;

    server->field = malloc(height * width * sizeof(char));
    memset(server->field, ' ', width * height * sizeof(char));


    int login_mqid = create_mq(SERVER_LOGIN_QUEUE);
    if(login_mqid != -1){
        server->login_mqid = login_mqid;

        int display_fifo_fd = create_write_fifo(SERVER_DISPLAY_PIPE);
        if(display_fifo_fd != -1){
            server->display_pipe_fd = display_fifo_fd;

            return server;
        }
    }
    free_server(server);
}

void free_server(server_t *server) {
    if(server->display_pipe_fd != -1){
        unlink(SERVER_DISPLAY_PIPE);
    }
    if(server->login_mqid != -1){
        delete_mq(server->login_mqid);
        unlink(SERVER_LOGIN_QUEUE);
    }

    for(int i = 0; i < MAX_CLIENTS; ++i){
        if(server->clients[i]){
            free_client(server->clients[i]);
        }
    }
    free(server->field);
    free(server);
}

void login_client(server_t* server, login_message_t message){
    if(get_client(server, message.id) == NULL){

        pos p = generate_new_pos(server);

        client_t* client = new_client(message.pid, message.id, p.x, p.y);
        if(client){
            set_client(server, client);

            msg_wrap msg = {
                .msg_type= LOGIN_SUCCESS,
                .buf.login_success.pos_x = client->pos_x,
                .buf.login_success.pos_y = client->pos_y,
            };

            if(send_to_mq(client->to_client_mqid, msg)){

                set_client_at_pos(server, client->pos_x, client->pos_y, client);

                printf("Successful login for client: %c with pid: %d\n", client->id, client->pid);
                update_display(server);
                return;
            }else{
                perror("msgsnd: Unable to send response to client: ");
            }

        }
    }else{
        printf("Client with id: %c is already logged in with pid: %d\n", message.id, get_client(server, message.id)->pid);
    }
    //error happened
    int to_client = get_client(server, message.id)->to_client_mqid;
    send_to_mq(to_client, (msg_wrap){ .msg_type=LOGIN_FAILED, .buf.empty={} });
}

void handle_login(server_t* server){
    msg_wrap msg;
    while (receive_from_mq_non_block(server->login_mqid, &msg)){
        switch (msg.msg_type) {
            case LOGIN:
                printf("Received login message with id: %c and pid %d\n", msg.buf.login.id, msg.buf.login.pid);
                login_client(server, msg.buf.login);
                break;
            default:
                fprintf(stderr, "Malformed message, unknown msg type: %ld\n", msg.msg_type);
                break;
        }
    }
}

void set_client_at_pos(server_t* server, int x, int y, client_t* client){
    server->field[server->width * y + x] = client->id;
}

void set_client(server_t* server, client_t* client){
    assert(client->id >= 'A' && client->id <= 'Z');
    server->clients[ client->id -'A'] = client;
}

char get_client_at_pos(server_t *server, int x, int y) {
    return server->field[server->width * y + x];
}

pos generate_new_pos(server_t *server) {
    int x = rand() % server->width;
    int y = rand() % server->height;
    while (get_client_at_pos(server, x, y) != ' '){
        x = rand() % server->width;
        y = rand() % server->height;
    }
    return (pos) { .x=x, .y=y };
}

void update_display(server_t *server) {
    printf("Printing to display process.\n");

    size_t line_size = server->width * sizeof(char) + 3;

    char* line_buf = malloc(line_size);
    memset(line_buf, '#', line_size);
    line_buf[line_size-1] = '\n';

    write(server->display_pipe_fd, line_buf, line_size);

    for(int y = 0; y < server->height; ++y){
        memcpy(line_buf+1, server->field + (server->width*y), server->width * sizeof(char));
        write(server->display_pipe_fd, line_buf, line_size);
    }
    memset(line_buf+1, '#',server->width* sizeof(char));
    write(server->display_pipe_fd, line_buf, line_size);

    //terminate msg with \0
    char end = '\0';
    write(server->display_pipe_fd, &end, 1);

    free(line_buf);
}

void handle_clients(server_t *server) {
    for(int i = 0; i < MAX_CLIENTS; ++i){
        if(server->clients[i]){
            handle_client(server, server->clients[i]);
        }
    }
}

void logout(server_t *server, client_t *client) {
    fprintf(stdout, "Logout request from client: %c with pid: %d\n", client->id, client->pid);
    set_at_pos(server, client->pos_x, client->pos_y, ' ');
    server->clients[client->id - 'A'] = NULL;
    free_client(client);
}

void client_action(server_t *server, client_t *client, char action) {
    assert(action == 'N' || action == 'E' || action == 'S' || action == 'W');
    fprintf(stdout, "Client: %c with pid: %d sent action: %c\n", client->id, client->pid, action);
    int dx = 0, dy = 0;

    switch (action) {
        case 'N': dx = 0; dy = -1;
            break;
        case 'E': dx = 1; dy = 0;
            break;
        case 'S': dx = 0; dy = 1;
            break;
        case 'W': dx = -1; dy = 0;
            break;
        default: assert(0);
    }

    int new_x = client->pos_x + dx,
        new_y = client->pos_y + dy;

    if(new_x < 0 || new_x >= server->width || new_y < 0 || new_y >= server->height){
        fprintf(stdout, "Collision Event: Client %c collided with border.\n", client->id);
        logout(server, client);
        return;
    }

    if(get_client_at_pos(server, new_x, new_y) !=  ' '){
        client_t* other = get_client(server, get_client_at_pos(server, new_x, new_y));
        fprintf(stdout, "Collision Event: Client %c collided with %c.\n", client->id, other->id);
        logout(server, client);
        logout(server, other);
        return;
    }

    set_at_pos(server, client->pos_x, client->pos_y, ' ');
    client->pos_x = new_x;
    client->pos_y = new_y;
    set_client_at_pos(server, client->pos_x, client->pos_y, client);

}

void handle_client(server_t *server, client_t *client) {
    msg_wrap msg;

    if(receive_from_mq_non_block(client->from_client_mqid, &msg)){
        switch (msg.msg_type) {
            case CLIENT_ACTION:
                client_action(server, client, msg.buf.client_action.action);
                update_display(server);
                break;
            case LOGOUT:
                logout(server, client);
                update_display(server);
                break;
            default:
                fprintf(stderr, "Unexpected message from client: %c with pid: %d.  msgtype (%ld)\n", client->id, client->pid, msg.msg_type);
                break;
        }
    }

}

void set_at_pos(server_t *server, int x, int y, char symbol) {
    server->field[y * server->width + x] = symbol;
}












