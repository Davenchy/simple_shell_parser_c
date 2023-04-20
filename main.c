#include <stdio.h>
#include <unistd.h>
#include "includes/command.h"
#include "includes/parser.h"

int main(void) {
	unsigned int count = 0;
	cmd_t *head;

loop:
	printf("> ");
	fflush(stdout);
	head = getcmds(STDIN_FILENO, &count);
	cmd_print(head);
	cmd_destroy(head);
	goto loop;
}
