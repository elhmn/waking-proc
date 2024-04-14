#include "lib.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

int parse_options(t_options *opts, int ac, char **av) {
    int c = 0;
    int option_index = 0;

    if (!opts) {
        return -1;
    }

    if (!opts) {
        printf("Error: opts is set to null");
        return -1;
    }

    struct option long_options[] = {{"verbose", no_argument, &opts->verbose, 1},
                                    {"mode", required_argument, NULL, 'm'},
                                    {0}};

    while ((c = getopt_long(ac, av, "vm:", long_options, &option_index)) !=
           -1) {
        switch (c) {
        case 1:
            if (optarg) {
                opts->verbose = atoi(optarg);

            }
            break;
        case 'm':
            opts->socket_mode = str_to_socket_mode(optarg);
            break;
        default:
            break;
        }
    }
    opts->optind = optind;
    return 0;
}

e_socket_mode str_to_socket_mode(char *str) {
    if (!str) {
        return SOCKET_MODE_UNKNOWN;
    }

    if (strcmp(str, "remote") == 0) {
        return SOCKET_MODE_REMOTE;
    } else if (strcmp(str, "local") == 0) {
        return SOCKET_MODE_LOCAL;
    } else {
        return SOCKET_MODE_UNKNOWN;
    }

    return SOCKET_MODE_UNKNOWN;
}

char *padding(int depth) {
    int i = 0;
    char *str = (char *)malloc((sizeof(char) * depth) + 1);

    for (i = 0; i < depth; i++) {
        str[i] = '\t';
    }
    str[i] = '\0';

    return str;
}

void print_options(t_options opts, int depth) {
    printfln_with_padding("opts {", depth - 1);
    printfln_with_padding("verbose: %d", depth, opts.verbose);
    printfln_with_padding("optind: %d", depth, opts.optind);
    printfln_with_padding("mode: %d", depth, opts.socket_mode);
    printfln_with_padding("}", depth - 1);
}

void printfln_with_padding(char *str, int depth, ...) {
    va_list arglist;
    va_start(arglist, depth);

    // TODO: fix this, not very safe
    // make it a little more dynamique
    char format[2048] = "";

    sprintf(format, "%s%s\n", padding(depth), str);
    vprintf(format, arglist);
    va_end(arglist);
}
