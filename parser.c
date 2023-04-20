#include "includes/parser.h"
#include "includes/command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int apply_token(char *token, ssize_t *tokensize, cmd_t *curr) {
	arg_t *arg;

	if (!*tokensize) return 1;
	arg = arg_create(token, *tokensize);
	if (!arg) return 0;
	*tokensize = 0;
	curr->arg = arg_push(curr->arg, arg);
	return 1;
}

int apply_cmd(char *token, ssize_t *tokensize, cmd_t **curr) {
	if (!apply_token(token, tokensize, *curr)) return 0;
	if (!(*curr)->arg) return 1;
	(*curr)->type = CMDTYPE_CMD;
	(*curr)->next = cmd_create(CMDTYPE_NULL);
	if (!(*curr)->next) return 0;
	*curr = (*curr)->next;
	return 1;
}

int apply_type(char type, char *token, ssize_t *tokensize, cmd_t **curr) {
	if (!apply_cmd(token, tokensize, curr)) return 0;

	if ((*curr)->arg) {
		(*curr)->next = cmd_create(type);
		if (!(*curr)->next) return 0;
		*curr = (*curr)->next;
	}
	(*curr)->type = type;
	(*curr)->next = cmd_create(CMDTYPE_NULL);
	if (!(*curr)->next) return 0;
	*curr = (*curr)->next;
	return 1;
}

cmd_t *getcmds(int fildes, unsigned int *count) {
	ssize_t bufsize = 0, tokensize = 0, ptr;
	cmd_t *head = NULL, *curr = NULL;
	char buf[BUFSIZE], token[BUFSIZE], str = 0;
	
	*count = 0;
	head = curr = cmd_create(CMDTYPE_NULL);
	if (!head) goto kill;
	
	bufsize = read(fildes, buf, BUFSIZE);
	if (bufsize == -1) {
		perror("Error");
		goto kill;
	}
	
	for (ptr = 0; ptr < bufsize; ptr++) {
		char c = buf[ptr];

		if (c == '\\') {
			c = buf[++ptr];
			goto apply_str_ignore;
		} else if (c == '"') {
			str = !str;
		} else if (str) {
			goto apply_str_ignore;
		} else if (strchr(" \t", c)) {
			if (!apply_token(token, &tokensize, curr)) goto kill;
		} else if (strchr("\n\r;", c) || c == 0 || c == EOF) {
			if (!apply_cmd(token, &tokensize, &curr)) goto kill;
		} else if (c == '|' && ptr < bufsize && buf[ptr + 1] != c) {
			if (!apply_type(CMDTYPE_PIPE, token, &tokensize, &curr)) goto kill;
		} else if (strchr("&|", c) && ptr < bufsize && buf[ptr + 1] == c) {
			char type = c == '&' ? CMDTYPE_AND : c == '|' ? CMDTYPE_OR : 0;
			if (!apply_type(type, token, &tokensize, &curr)) goto kill;
		} else {
apply_str_ignore:
			token[tokensize++] = c;
		}
	}
	
	return head;
kill:
	cmd_destroy(head);
	return NULL;
}
