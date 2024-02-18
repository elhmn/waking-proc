#include "lib.h"
#include "waking-proc.h"
#include <stdio.h>
#include <stdlib.h>

int main(int ac, char **av) {
    char *buf = NULL;
    t_options opts = {
        .socket_mode = SOCKET_MODE_LOCAL, .verbose = 0, .optind = 0};

    if (parse_options(&opts, ac, av)) {
        fprintf(stderr, "Error: failed to parse options\n");
        return 1;
    }

    if (opts.verbose) {
        print_options(opts, 1);
    }

    buf = list_processes();
    if (buf) {
        if (opts.verbose) {
            printf("%s", buf);
        }
        free(buf);
    }
}
