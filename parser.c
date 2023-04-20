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
		} else if (c == '$' && n && l != '\\') {
			if (!apply_token(p)) goto kill;
			p->tokensize = strcspn(p->buf + p->ptr, " \n\t\r\"");
			memcpy(p->token, p->buf + p->ptr, p->tokensize);
			if (p->tokensize > 0)
				p->ptr += p->tokensize - 1;
			if (!apply_token(p)) goto kill;
		} else if (p->str) {
			goto apply_str_ignore;
		} else if (strchr(" \t", c)) {
			if (!apply_token(p)) goto kill; /* handle end of var */
		} else if (strchr("\n\r;", c) || c == 0 || c == EOF) {
			if (!apply_cmd(p)) goto kill; /* handle end of var */
		} else if (c == '|' && l != c && n != c) {
			if (!apply_type(CMDTYPE_PIPE, p, NULL)) goto kill;
		} else if (strchr("&|", c) && l != c && n == c) {
			char type = c == '&' ? CMDTYPE_AND : c == '|' ? CMDTYPE_OR : 0;
			if (!apply_type(type, p, NULL)) goto kill;
			p->ptr++;
		} else if (c == '#') {
			p->ptr += strcspn(p->buf + p->ptr + 1, "\n\t\r");
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
	int read_state = 0;
	parser_t parser;

	parser.bufsize = 0;
	parser.tokensize = 0;
	parser.head = NULL;
	parser.curr = NULL;
	parser.str = 0;
	
	parser.head = parser.curr = cmd_create(CMDTYPE_NULL);
	if (!parser.head) goto kill;
	
	parser.bufsize = read(fildes, parser.buf, BUFSIZE);
	if (!parser.bufsize) goto end;
	else if (parser.bufsize == -1) {
		perror("Error");
		goto kill;
	}

	read_state = parse(&parser);
	if (!read_state) goto kill;
end:
	return parser.head;
kill:
	cmd_destroy(parser.head);
	return NULL;
}
