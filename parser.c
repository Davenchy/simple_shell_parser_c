#include "includes/parser.h"
#include "includes/command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int apply_token(parser_t *p) {
	arg_t *arg;

	if (!p->tokensize) return 1;
	arg = arg_create(p->token, p->tokensize);
	if (!arg) return 0;
	p->tokensize = 0;
	p->curr->arg = arg_push(p->curr->arg, arg);
	p->curr->count++;
	return 1;
}

int apply_cmd(parser_t *p) {
	if (!apply_token(p)) return 0;
	if (!p->curr->arg) return 1;
	p->curr->type = CMDTYPE_CMD;
	p->curr->next = cmd_create(CMDTYPE_NULL);
	if (!p->curr->next) return 0;
	p->curr = p->curr->next;
	return 1;
}

int apply_type(char type, parser_t *p, arg_t *arg) {
	if (!apply_cmd(p)) return 0;

	if (p->curr->arg) {
		p->curr->next = cmd_create(type);
		if (!p->curr->next) return 0;
		p->curr = p->curr->next;
	}
	p->curr->type = type;
	p->curr->arg = arg;
	p->curr->next = cmd_create(CMDTYPE_NULL);
	if (!p->curr->next) return 0;
	p->curr = p->curr->next;
	return 1;
}

int parse(parser_t *p) {
	for (p->ptr = 0; p->ptr < p->bufsize; p->ptr++) {
		char l = p->ptr > 0 ? p->buf[p->ptr - 1] : 0;
		char c = p->buf[p->ptr];
		char n = p->ptr < p->bufsize ? p->buf[p->ptr + 1] : 0;

		if (c == '\\') {
			c = p->buf[++p->ptr];
			goto apply_str_ignore;
		} else if (c == '"') {
			p->str = !p->str;
		} else if (p->str) {
			goto apply_str_ignore;
		} else if (strchr(" \t", c)) {
			if (!apply_token(p)) goto kill;
		} else if (strchr("\n\r;", c) || c == 0 || c == EOF) {
			if (!apply_cmd(p)) goto kill;
			if (c == EOF) return 2;
		} else if (c == '|' && l != c && n != c) {
			if (!apply_type(CMDTYPE_PIPE, p, NULL)) goto kill;
		} else if (strchr("&|", c) && l != c && n == c) {
			char type = c == '&' ? CMDTYPE_AND : c == '|' ? CMDTYPE_OR : 0;
			if (!apply_type(type, p, NULL)) goto kill;
			p->ptr++;
		} else {
apply_str_ignore:
			p->token[p->tokensize++] = c;
		}
	}
	return 1;
kill:
	return 0;
}

cmd_t *getcmds(int fildes) {
	parser_t parser;

	parser.bufsize = 0;
	parser.tokensize = 0;
	parser.head = NULL;
	parser.curr = NULL;
	parser.str = 0;
	
	parser.head = parser.curr = cmd_create(CMDTYPE_NULL);
	if (!parser.head) goto kill;
	
	parser.bufsize = read(fildes, parser.buf, BUFSIZE);
	if (parser.bufsize == -1) {
		perror("Error");
		goto kill;
	}

	if (!parse(&parser)) goto kill;

	return parser.head;
kill:
	cmd_destroy(parser.head);
	return NULL;
}
