//
// Created by daniel on 11.05.18.
//
#include <stdio.h>
#include <memory.h>
#include "messages.h"

int create_write_fifo(const char *const pipe_name) {
    int pipe = mkfifo(pipe_name, 0666);
    printf("created fifo: %d\n", pipe);
    int fd = open(pipe_name, O_WRONLY);
    printf("opened fifo: %d\n", fd);
    return fd;
}

int open_read_fifo(const char *const pipe_name) {
    open(pipe_name, O_RDONLY);
}

int create_mq(const char *const mq_name) {
    open(mq_name, O_CREAT, 0644);
    key_t file_key = ftok(mq_name, UNIQUE_MQ_KEY);
    return msgget(file_key, 0644 | IPC_CREAT);
}

int connect_mq(const char *const mq_name) {
    open(mq_name, 0666);
    key_t file_key = ftok(mq_name, UNIQUE_MQ_KEY);
    return msgget(file_key, 0644);
}

int waiting_messages(int mqid) {
    struct msqid_ds buf;
    msgctl(mqid, IPC_STAT, &buf);
    if(buf.msg_qnum <= 0) return (int) buf.msg_qnum;
    else {
        perror("msgctl: getting mq info");
        exit(1);
    }
}

int mq_exists(int mqid){
    if(msgget(mqid, 0) != -1){
        return 1;
    }else if(errno == ENOENT){
        return 0;
    } else {
        perror("msgget: checking for exists: ");
        exit(1);
    }

}

void delete_mq(int mqid) {
    msgctl(mqid, IPC_RMID, 0);
}

int send_to_mq(int mqid, msg_wrap msg) {
    if(msgsnd(mqid, &msg, MSG_SIZE, 0) != -1){
        return 1;
    } else {
        perror("msgsnd: Error sending message:");
        return 0;
    }
}

int receive_from_mq_block(int mqid, msg_wrap *msg){
    if(msgrcv(mqid, msg, MSG_SIZE, 0, 0) != -1) {
        return 1;
    } else {
        perror("msgrcv: Unable to receive msg blocking:");
        return 0;
    }
}

int receive_from_mq_non_block(int mqid, msg_wrap *msg) {
    if(msgrcv(mqid, msg, MSG_SIZE, 0, IPC_NOWAIT) != -1){
        return 1;
    } else if(errno == ENOMSG) {
        return 0;
    } else {
        perror("msgrcv: Unable to receive msg non blocking:");
        exit(1);
    }
}

const char *get_from_client_str(char id) {
    char* buf = malloc(sizeof(SERVER_FROM_CLIENT_QUEUE));
    strcpy(buf, SERVER_FROM_CLIENT_QUEUE);
    buf[sizeof(SERVER_FROM_CLIENT_QUEUE)-2] = id;
    return buf;
}

const char *get_to_client_str(char id) {
    char* buf = malloc(sizeof(SERVER_TO_CLIENT_QUEUE));
    strcpy(buf, SERVER_TO_CLIENT_QUEUE);
    buf[sizeof(SERVER_TO_CLIENT_QUEUE)-2] = id;
    return buf;
}