//
// Created by daniel on 11.05.18.
//

#include <stdio.h>
#include <signal.h>
#include <zconf.h>
#include <time.h>
#include <assert.h>
#include "messages.h"
#include "server.h"


static int run = 1;

/**
 * handles SIGINT and SIGTERM
 * @param sig code of the signal
 */
void signal_handler(int sig){
    printf("Requesting shutdown with signal: %d", sig);
    run = 0;
}

/**
* prints usage information
*/
void print_usage(char *program_name) {
    fprintf(stderr, "Usage: %s -x NUM -y NUM\n", program_name);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int c;
    int error = 0;
    char *program_name = argv[0];
    int width_set = 0;
    int height_set = 0;
    int width = 0;
    int height = 0;

    while ((c = getopt(argc, argv, "x:y:")) != EOF) {
        switch (c) {
            case 'x':
                if (width_set) {
                    fprintf(stderr, "multiple usage of flags is not allowed\n");
                    error = 1;
                    break;
                }
                width_set = 1;
//                printf("Optarg is: %d", optarg);
                width = atoi(optarg);
//                printf("width set to: %d\n", width);
                break;
            case 'y':
                if (height_set) {
                    fprintf(stderr, "multiple usage of flags is not allowed\n");
                    error = 1;
                    break;
                }
                height_set = 1;
//                printf("Optarg is: %d", optarg);
                height= atoi(optarg);
//                printf("height set to: %d\n", height);
                break;
                // invalid option
            case '?':
                error = 1;
                break;
            default:
                //cant reach here
                assert(0); //assert used when this code part can normally be never reached
        }
    }

    // options were false
    if (error) {
        print_usage(program_name);
    }

    // false count of options
    if ((argc != 5 || optind != 5)) {
        fprintf(stderr, "false number of arguments\n");
        print_usage(program_name);
    }

    ////// original logic ///////
    // TODO: what for srand?
    srand(time(NULL));
    signal(15, signal_handler);
    signal(2, signal_handler);

    server_t* server = new_server(width, height);

    while (run){
        handle_login(server);
        handle_clients(server);

        // TODO: why sleep
        usleep(100000);
    }

    free_server(server);
}
