#include <stdio.h>
#include <unistd.h>
#include "includes/command.h"
#include "includes/parser.h"

int main(void) {
	cmd_t *head;

loop:
	printf("> ");
	fflush(stdout);
	head = getcmds(STDIN_FILENO);
	cmd_print(head);
	cmd_destroy(head);
	goto loop;
}
