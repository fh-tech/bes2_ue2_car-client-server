//
// Created by daniel on 02.05.18.
//



#include <stdio.h>
#include <malloc.h>
#include <sys/msg.h>
#include <memory.h>
#include "server.h"

#include "../shared/client_msg_types.h"

int try_connect(char* unique_str){
    int tmp_file = open(unique_str, O_CREAT, 0666);
    key_t file_key = ftok(unique_str, QUEUE_MSG_KEY);
    return msgget(file_key, 0644 | IPC_CREAT);
}

server_t *init_server(server_t *server, void (*free)(void *)) {

    server->width = 0;
    server->height = 0;

    memset(&(server->clients), 0 ,sizeof(server->clients));

    int login_queue_id = try_connect(SERVER_LOGIN);

    if(login_queue_id != -1){

        server->login_queue_id = login_queue_id;

    }else{
        free_server(server, free);
        return NULL;
    }


}



int free_server(server_t *server, void (*free)(void *)) {

    remove(SERVER_LOGIN);
    msgctl(server->login_queue_id, IPC_RMID, NULL);

    if(free) {
        for(int i = 0; i < 26; i++) {
            if(server->clients[i]){
                free(&(server->clients[i]));
            }
        }
        free(server);
    }
}

int send_to_client(server_t *server, client_msg msg) {
    char buf[] = "Hello World";
    return msgsnd(server->login_queue_id, &buf, sizeof(buf), 0 ) != -1;

}

const char *display_state(server_t *server) {
    static int width = 13;
    static char field[] = {
            "############\n"
            "#          #\n"
            "#          #\n"
            "#          #\n"
            "#          #\n"
            "#          #\n"
            "#          #\n"
            "#          #\n"
            "#          #\n"
            "#          #\n"
            "#          #\n"
            "############\n"
    };
    static char msg[sizeof(field)] = {0};
    memcpy(&msg,&field, sizeof(field));
    //TODO: Draw stuff
    return field;
}

msgqnum_t waiting_messages(int mesg_id) {
    struct msqid_ds buf;
    if(msgctl(mesg_id, IPC_STAT, &buf) != -1) {
        return buf.msg_qnum;
    }else{
        perror("Checking for message num");
        exit(1);
    }
}

void login_client(server_t *server, char id, pid_t pid) {
    char to_client[] = SERVER_CLIENT_TO;
    to_client[sizeof(SERVER_CLIENT_TO) - 2] = id;

    char from_client[] = SERVER_CLIENT_FROM;
    from_client[sizeof(SERVER_CLIENT_FROM) - 2] = id;

    int to_client_q = try_connect(to_client);
    int from_client_q = try_connect(from_client);

    if(to_client_q >= -1 && from_client_q >= -1){
        if(server->clients[id-'A'] != NULL) {
            server->clients[id - 'A'] = malloc(sizeof(client));

            *server->clients[id - 'A'] = (client) {
                    .id = id,
                    .pid = pid,
                    .dir = 0,
                    .to_client_msg_q=to_client_q,
                    .from_client_msg_q=from_client_q,
                    .x=0,
                    .y=0,
            };

            client_msg msg = {
                    .msg_type=SUCCESS_LOGIN,
                    .payload.success_login.x = 0,
                    .payload.success_login.y = 0,
            };
            msgsnd(to_client_q, &msg, sizeof(msg), 0);
        }else{
            client_msg msg =  {
                    .msg_type=FAILED_LOGIN,
            };
            msgsnd(to_client_q, &msg, sizeof(msg), 0);
        }
    }else{
        fprintf(stderr, "Unable to establish queue to client %c\n", id);
        perror("error: ");
    }

}

