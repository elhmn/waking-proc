#ifdef __APPLE__
#include <libproc.h>
#endif

#include "waking-proc.h"
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

typedef struct str {
    char *buf;
    size_t size;
    size_t cap;
} t_str;

t_str *new_str(size_t size) {
    t_str *s = (t_str *)malloc(sizeof(t_str));
    if (!s) {
        return NULL;
    }

    if (!(s->buf = (char *)malloc((size) * sizeof(char)))) {
        return NULL;
    }
    memset(s->buf, 0, size);
    s->size = 0;
    s->cap = size;
    return s;
}

void append_str(t_str *s, char *buf) {
    int len = strlen(buf);

    if (s->cap - s->size < (size_t)len) {
        s->buf = (char *)realloc(s->buf, (s->cap + BUFFER_SIZE) * sizeof(char));
        s->cap += BUFFER_SIZE;
    } else {
        strncat(s->buf, buf, len);
    }

    s->size = strlen(s->buf);
}

// Function to check if the directory name is numeric
int is_numeric(const char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isdigit(name[i]))
            return 0;
    }
    return 1;
}

// Function to split a string
char **str_split(char *str, const char delim) {
    char *orig = str;
    int i = 0;
    char **arr = NULL;

    // Count the number of delimiters in the string
    while (str) {
        if ((str = strchr(str, delim))) {
            str++;
            i++;
        }
    }

    arr = (char **)malloc((i + 1) * sizeof(char *));
    if (!arr) {
        return NULL;
    }
    arr[i] = NULL;

    str = orig;
    i = 0;
    while (str) {
        char *tmp = str;
        if ((str = strchr(str, delim))) {
            *str = '\0';
            arr[i] = strdup(tmp);
            str++;
            i++;
        }
    }
    return arr;
}

void do_something(void) { printf("waking proc library\n"); }
#ifdef _WIN32
// Define windows functions here
#elif __linux__

/*
 * Read the /proc/[pid]/stat file, and store the contents in the buffer
 */
static int read_proc_stat(char *pid, char *buf) {
    FILE *fp = NULL;
    if (!pid || !buf) {
        return -1;
    }

    snprintf(buf, BUFFER_SIZE, "/proc/%s/stat", pid);
    if (!(fp = fopen(buf, "r"))) {
        return -1;
    }

    //     bzero(buf, BUFFER_SIZE);
    fread(buf, BUFFER_SIZE, 1, fp);
    fclose(fp);
    return 0;
}

static void proc_init(t_proc *proc) {
    proc->name = NULL;
    proc->pid = NULL;
    proc->ppid = NULL;
    proc->uid = NULL;
    proc->gid = NULL;
    proc->state = NULL;
    proc->time = NULL;
    proc->utime = NULL;
    proc->stime = NULL;
    proc->priority = NULL;
    proc->command = NULL;
}

static void proc_free(t_proc **proc) {
    t_proc *p = *proc;
    if (!proc || !*proc) {
        return;
    }

    if (p->name)
        free(p->name);
    if (p->pid)
        free(p->pid);
    if (p->ppid)
        free(p->ppid);
    if (p->uid)
        free(p->uid);
    if (p->gid)
        free(p->gid);
    if (p->state)
        free(p->state);
    if (p->time)
        free(p->time);
    if (p->utime)
        free(p->utime);
    if (p->stime)
        free(p->stime);
    if (p->priority)
        free(p->priority);
    if (p->command)
        free(p->command);
    free(*proc);
    *proc = NULL;
}

static void free_arr(char **arr) {
    char **tmp = arr;
    if (!arr) {
        return;
    }

    while (*tmp) {
        free(*tmp++);
    }
    free(arr);
}


/*
 *  get_proc_info, populate the proc structure with information extracted from
 * various files in /proc and return the structure.
 *
 *  The caller is responsible for freeing the memory allocated for the proc
 * structure
 */
static t_proc *get_proc_info(char *pid, char *buf) {
    t_proc *proc = NULL;
    if (!(proc = (t_proc *)malloc(sizeof(t_proc)))) {
        return NULL;
    }
    proc_init(proc);

    bzero(buf, BUFFER_SIZE);
    if (read_proc_stat(pid, buf) < 0) {
        printf("Failed to read /proc/%s/stat\n", pid);
        free(proc);
        return NULL;
    }

    char **arr = str_split(buf, ' ');

    proc->pid = strdup(pid);
    proc->name = strdup(arr[1]);

    free_arr(arr);
    return proc;
}

static void proc_to_string(t_proc *proc, char *buf) {
    if (!proc) {
        return;
    }
    sprintf(buf, "{");
    sprintf(buf, "%s\"name\":\"%s\",", buf, proc->name);
    sprintf(buf, "%s\"pid\":\"%s\"", buf, proc->pid);
    sprintf(buf, "%s}", buf);
}

static char *linux_list_processes(void) {
    struct dirent *entry = NULL;
    DIR *dp = NULL;
    char buf[BUFFER_SIZE] = {0};
    t_proc *proc = NULL;
    char *str = NULL;
    t_str *s = NULL;
    char *pid = NULL;

    if (!(s = new_str(BUFFER_SIZE))) {
        perror("Failed to allocate a new string");
        return NULL;
    }

    if (!(dp = opendir("/proc"))) {
        perror("Failed to open /proc");
        return NULL;
    }

    append_str(s, "[");
    while ((entry = readdir(dp))) {
        // ensure we are dealing with a process directory
        if (entry->d_type == DT_DIR && is_numeric(entry->d_name)) {
            if (s->size && s->buf[s->size - 1] == '}') {
                append_str(s, ",");
            }

            pid = entry->d_name;
            if (!(proc = get_proc_info(pid, buf))) {
                continue;
            }
            proc_to_string(proc, buf);
            proc_free(&proc);
            append_str(s, buf);
        }
    }
    append_str(s, "]");

    str = s->buf;
    free(s);
    closedir(dp);
    return str;
}

#elif __APPLE__
static char *apple_list_processes(void) {
    pid_t pids[2048];
    int bytesReturned;
    int numPids;
    t_str *s = NULL;
    s = new_str(BUFFER_SIZE);
    char *str = NULL;

    bytesReturned = proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));
    if (bytesReturned <= 0) {
        fprintf(stderr, "Failed to list pids\n");
        return NULL;
    }

    numPids = bytesReturned / sizeof(pid_t);
    for (int i = 0; i < numPids; ++i) {
        char processName[PROC_PIDPATHINFO_MAXSIZE];
        struct proc_taskinfo taskInfo;
        pid_t pid = pids[i];
        if (pid == 0)
            continue;

        if (proc_name(pid, processName, sizeof(processName)) <= 0) {
            continue;
        }

        if (proc_pidinfo(pid, PROC_PIDTASKINFO, 0, &taskInfo,
                         sizeof(taskInfo)) <= 0) {
            continue;
        }

        char buf[BUFFER_SIZE];

        // PID;RSS(kb);Name
        snprintf(buf, BUFFER_SIZE, "%d;%llu;%s\n", pid,
                 taskInfo.pti_resident_size / 1024, processName);
        append_str(s, buf);
    }

    str = s->buf;
    return str;
}
#endif

char *list_processes(void) {
#ifdef _WIN32
    printf("Not supported for Windows\n");
    return NULL;
#elif __linux__
    return linux_list_processes();
#elif __APPLE__
    return apple_list_processes();
#endif
}
