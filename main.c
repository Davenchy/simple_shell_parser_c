#include <stdio.h>
#include <unistd.h>
#include "includes/command.h"
#include "includes/parser.h"

int main(void) {
	cmd_t *head;
	int fd = STDIN_FILENO;

loop:
	if (isatty(fd))
		printf("> ");
	fflush(stdout);
	head = getcmds(fd);
	cmd_print(head);
	cmd_destroy(head);
	if (isatty(fd))
		goto loop;
	return 0;
}
