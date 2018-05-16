//
// Created by daniel on 11.05.18.
//

#include <messages.h>
#include <sys/poll.h>
#include <stdio.h>
#include <zconf.h>

#define BUF_SIZE 128

int main(){
    int fd = open_read_fifo(SERVER_DISPLAY_PIPE);
    printf("Opened fifo to read: %d\n", fd);
    char buf[BUF_SIZE+1] = {0};


    int clear = 0;

    while (1){
        ssize_t i = read(fd, buf, BUF_SIZE);

        if(i == 0) break;

        if(clear){
            clear = 0;
            system("clear");
        }

        printf(buf);
        if(i != BUF_SIZE){
            printf("\n");
            clear = 1;
        }
    }
    return 0;
}