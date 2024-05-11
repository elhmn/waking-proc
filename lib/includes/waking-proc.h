#ifndef WAKING_PROC_H
# define WAKING_PROC_H

void do_something(void);
char *list_processes(void);

typedef struct proc {
    char *name;
    char *pid;
    char *ppid;
    char *uid;
    char *gid;
    char *state;
    char *time;
    char *utime;
    char *stime;
    char *priority;
    char *command;
} t_proc;

#endif // WAKING_PROC_H
