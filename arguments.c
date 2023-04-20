#include "includes/command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void arg_destroy(arg_t *arg) {
	arg_t *next;

loop:
	if (!arg)
		return;
	next = arg->next;
	if (arg->token) free(arg->token);
	free(arg);
	arg = next;
	goto loop;
}

arg_t *arg_create(char *token, size_t size) {
	arg_t *arg = calloc(1, sizeof(arg_t));

	if (!arg)
		return NULL;
	arg->size = size;
	arg->token = strndup(token, size);
	if (!arg->token) {
		free(arg);
		return NULL;
	}

	return arg;
}

arg_t *arg_push(arg_t *head, arg_t *arg) {
	arg_t *curr = head;

	if (!head)
		return arg;

	while (curr->next)
		curr = curr->next;
	curr->next = arg;
	return head;
}

void arg_print(arg_t *head) {
begin:
	if (!head) {
		printf("|");
		putchar(10);
		return;
	}
	if (head->token)
		printf("[%s]->", head->token);
	head = head->next;
	goto begin;
}
