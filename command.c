#include <stdio.h>
#include <stdlib.h>
#include "includes/command.h"

cmd_t *cmd_create(char type) {
	cmd_t *cmd = calloc(1, sizeof(cmd_t));

	if (!cmd)
		return NULL;
	cmd->type = type;
	return cmd;
}

void cmd_destroy(cmd_t *head) {
	cmd_t *next;

loop:
	if (!head)
		return;
	next = head->next;
	if (head->arg) arg_destroy(head->arg);
	free(head);
	head = next;
	goto loop;
}

cmd_t *cmd_get_last(cmd_t *head) {
	if (!head) return NULL;
	else if (!head->next) return head;
	else return cmd_get_last(head->next);
}

cmd_t *cmd_push(cmd_t *head, cmd_t *cmd) {
	if (!head) return NULL;
	if (head->next) cmd_get_last(head)->next = cmd;
	else head->next = cmd;
	return head;
}

void cmd_print(cmd_t *head) {
	while (head) {
		switch (head->type) {
			case CMDTYPE_NULL:
				puts("[NULL]");
				break;
			case CMDTYPE_CMD:
				printf("[CMD]=>");
				arg_print(head->arg);
				break;
			case CMDTYPE_OR:
				puts("[OR]");
				break;
			case CMDTYPE_AND:
				puts("[AND]");
				break;
			case CMDTYPE_PIPE:
				puts("[PIPE]");
				break;
		}
		head = head->next;
	}
}
