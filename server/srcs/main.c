#include <stdio.h>
#include <stdlib.h>
#include "waking-proc.h"

int main(int ac, char **av) {
	char *buf = NULL;
	(void)ac;
	(void)av;
	do_something();
	buf = list_processes();
	if (buf) {
		printf("%s", buf);
		free(buf);
	}
}
