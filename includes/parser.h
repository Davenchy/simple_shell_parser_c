#ifndef __PARSER_H__
#define __PARSER_H__
#define BUFSIZE 1024
#include <stdio.h>
#include "command.h"

typedef struct parser_s {
	ssize_t bufsize, tokensize, ptr;
	cmd_t *head, *curr;
	char buf[BUFSIZE], token[BUFSIZE], str;
} parser_t;

cmd_t *getcmds(int fildes);

#endif
