//
// Created by daniel on 11.05.18.
//

#ifndef CAR_CLIENT_SERVER2_MESSAGES_H
#define CAR_CLIENT_SERVER2_MESSAGES_H

#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <asm/errno.h>
#include <errno.h>
#include <sys/stat.h>

#define UNIQUE_MQ_KEY 34057869

#define SERVER_LOGIN_QUEUE "/tmp/car-client-server_login"
#define SERVER_DISPLAY_PIPE "/tmp/car-client-server_display"

#define SERVER_TO_CLIENT_QUEUE "/tmp/car-client-server_client-to-$"
#define SERVER_FROM_CLIENT_QUEUE "/tmp/car-client-server_client-from-$"

#define MSG_SIZE sizeof(message)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Possible message payloads
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


typedef struct logout_message_s {

    char id;

} logout_message_t;



typedef struct login_message_s {

    char id;
    pid_t pid;

} login_message_t;



typedef struct login_success_message_s {

    int pos_x;
    int pos_y;

} login_success_message_t;



typedef struct client_action_message_s {

    char action;

} client_action_message_t;



typedef struct emtpy_s {
} empty_t;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Message type:
 * A message consist of a msg_type which specifies the type of message
 * and a buf which is a union of possible message payloads.
 * Each different payload has exactly one message type to identify it.
 *
 * This is basically a tagged union.
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef union {
    empty_t empty;
    login_message_t login;
    logout_message_t logout;
    login_success_message_t login_success;
    client_action_message_t client_action;
} message;


typedef enum {

    LOGIN_FAILED = 1001,
    LOGIN = 1,
    LOGOUT = 2,
    CLIENT_ACTION = 3,
    LOGIN_SUCCESS = 2

} message_type;

typedef struct {
    long msg_type;
    message buf;
} msg_wrap;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Utility functions for easier message queue and fifo work
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int create_write_fifo(const char* const pipe_name);

int open_read_fifo(const char* const pipe_name);

int create_mq(const char* const mq_name);

void delete_mq(int mqid);

int connect_mq(const char* const mq_name);

int waiting_messages(int mqid);

int send_to_mq(int mqid, msg_wrap msg);

int receive_from_mq_non_block(int mqid, msg_wrap* msg);

int receive_from_mq_block(int mqid, msg_wrap *msg);

const char *get_from_client_str(char id);

const char *get_to_client_str(char id);


#endif //CAR_CLIENT_SERVER2_MESSAGES_H
