#include <libproc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

typedef struct str {
	char *buf;
	size_t size;
	int32_t cap;
} t_str;

t_str *new_str(size_t size) {
	t_str *s = (t_str*)malloc(sizeof(t_str));
    if (!(s->buf = (char *)malloc((size) * sizeof(char)))) {
        return NULL;
    }
    memset(s->buf, 0, size);
	s->size = size;
	s->cap = size;
	return s;
}

void append_str(t_str *s, char *buf) {
	int len = strlen(buf);
	if (len >= s->cap) {
		s->size += BUFFER_SIZE;
		s->buf = (char*)realloc(s->buf, s->size * sizeof(char));
		s->cap = s->size;
	} else {
		strncat(s->buf, buf, len);
		s->cap = s->size - strlen(s->buf);
	}
}

void do_something(void) { printf("waking proc library\n"); }

static char *apple_list_processes(void) {
    pid_t pids[2048];
    int bytesReturned;
    int numPids;
    t_str *s = NULL;
    s = new_str(BUFFER_SIZE);

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
        snprintf(buf, BUFFER_SIZE, "%d;%llu;%s\n", pid, taskInfo.pti_resident_size / 1024,
               processName);
        append_str(s, buf);
    }
    return s->buf;
}

char *list_processes(void) {
#ifdef _WIN32
    printf("Not supported for Windows\n");
    return -1;
#elif __linux__
    printf("Not supported for linux\n");
    return -1;
#elif __APPLE__
	return apple_list_processes();
#endif
}
