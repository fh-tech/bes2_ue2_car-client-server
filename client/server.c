//
// Created by daniel on 05.05.18.
//
#include "server.h"


int try_connect(char *unique_str) {
    int tmp_file = open(unique_str, 0666);
    key_t file_key = ftok(unique_str, QUEUE_MSG_KEY);
    return msgget(file_key, 0644);
}


server_t login(char id, pid_t pid) {
    int login_q = try_connect(SERVER_LOGIN);
    if(login_q >= 0){
        client_msg  msg = {
                .msg_type= LOGIN,
                .payload.login_msg.client_id=id,
                .payload.login_msg.client_pid=pid,
        };
        if(msgsnd(login_q, &msg, sizeof(msg), 0) != -1){
            usleep(100000);

            char from_server[] = SERVER_CLIENT_TO;
            from_server[sizeof(SERVER_CLIENT_TO) - 2] = id;

            char to_server[] = SERVER_CLIENT_FROM;
            to_server[sizeof(SERVER_CLIENT_FROM) - 2] = id;

            int to_server_q   = try_connect(to_server);
            int from_server_q = try_connect(from_server);

            if(to_server_q >= 0 && from_server_q >= 0){
                printf("Login successful!\n");
                return (server_t) {
                        .from_server_msg_id = from_server_q,
                        .to_server_msg_id = to_server_q
                };
            }else{
                perror("Could not connect to client server queue");
                exit(1);
            }
        }else{
            perror("Could not send login request to server");
            exit(1);
        }
    }


}
