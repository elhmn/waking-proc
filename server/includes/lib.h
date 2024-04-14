#ifndef LIB_H
#define LIB_H

# define UNIX_SOCKET_PATH "/tmp/waking-proc-socket"

typedef enum {
    SOCKET_MODE_UNKNOWN,
    SOCKET_MODE_LOCAL,
    SOCKET_MODE_REMOTE
} e_socket_mode;

typedef struct s_options {
    int verbose;
    /* optind index of the last string parsed from argv */
    int optind;

    /*
     * The type of socket we want to use (local or remote)
	*/
    e_socket_mode socket_mode;
} t_options;

int parse_options(t_options *opts, int ac, char **av);
e_socket_mode str_to_socket_mode(char *str);
void printfln_with_padding(char *str, int depth, ...);
void print_options(t_options opts, int depth);

#endif /* LIB_H */
