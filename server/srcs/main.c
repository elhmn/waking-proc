#include "lib.h"
#include "config.h"
#include <stdio.h>
#include "waking-proc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int ac, char **av) {
    // I need to handle signals to make sure that the program is shutdown
    // correctly

    char *buf = NULL;
    t_options opts = {
        .socket_mode = SOCKET_MODE_LOCAL, .verbose = 0, .optind = 0};

    if (parse_options(&opts, ac, av)) {
        fprintf(stderr, "Error: failed to parse options\n");
        return 1;
    }

    // If the mode is local, I need to open a unix domain socket to start
    // listening for incoming connections
    //
    // If the mode is remote, I need to open a tcp socket to start listening for
    // incoming connections

    // For the first version of the program, each connection will be handled in a separate process
    // I will use fork to create a new process for each connection
    // The parent process will continue to listen for incoming connections

    // I should also spin up a thread to look for running processes
    // and whenever I am done reading the process tree, I should send
    // json formatted data to the client that is listening on every openeed connections

    while (1)  {
        printf("Process table lookup...\n");
        buf = list_processes();
        if (buf) {
            if (opts.verbose) {
                printf("%s\n", buf);
            }
            free(buf);
        }
        printf("Process able lookup done\n");
        usleep(DEFAULT_POLL_INTERVAL);
    }
}
