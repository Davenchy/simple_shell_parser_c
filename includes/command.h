#ifndef __COMMAND_H__

#include <stddef.h>

#define __COMMAND_H__
#define CMDTYPE_NULL 0
#define CMDTYPE_CMD 1
#define CMDTYPE_VAR 2
#define CMDTYPE_AND 3
#define CMDTYPE_OR 4
#define CMDTYPE_PIPE 5

typedef struct arg_s {
	char *token;
	size_t size;
	struct arg_s *next;
} arg_t;

typedef struct cmd_s {
	arg_t *arg;
	size_t count;
	struct cmd_s *next;
	char type;
} cmd_t;

cmd_t *cmd_create(char type);
void cmd_destroy(cmd_t *head);
cmd_t *cmd_get_last(cmd_t *head);
cmd_t *cmd_push(cmd_t *head, cmd_t *cmd);
void cmd_print(cmd_t *head);

void arg_destroy(arg_t *arg);
arg_t *arg_create(char *token, size_t size);
arg_t *arg_push(arg_t *head, arg_t *arg);
void arg_print(arg_t *head);

#endif
