//
// Created by daniel on 5/7/18.
//

#include <stdio.h>
#include <sys/stat.h>
#include "../shared/client_msg_types.h"

int main(){
    //mkfifo(SERVER_DISPLAY, 0666);
    int fd = open(SERVER_DISPLAY, O_RDONLY);
    printf("opend pipe %s: %d\n", SERVER_DISPLAY, fd);

    char c;
    scanf(" %c", &c);

    return 0;
}
