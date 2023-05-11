#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "includes/lexer.h"
#include "includes/token.h"

void print_token(token_t *token)
{
	if (!token)
		return;
	if (token->type == T_TEXT)
		printf("[%s](%lu)\n", token->text, token->size);
	else if (token->type == T_VAR)
		printf("VAR[%s](%lu)\n", token->text, token->size);
	else if (token->type == T_AND)
		printf("AND\n");
	else if (token->type == T_OR)
		printf("OR\n");
	else if (token->type == T_PIPE)
		printf("PIPE\n");
	else if (token->type == T_CMD)
		puts("CMD");
	else if (token->type == T_ERR) {
		fprintf(stderr, "Error!\n");
		exit(1);
	}
	else
		printf("UNKNOWN_TYPE(%d)\n", token->type);
}

/**
 * main - simple command tokenizer
 *
 * Return: status code
 */
int main(void) {
	token_t *token;
	char buf[1024];
	ssize_t nread;
	int fd = STDIN_FILENO;

loop:
	/* print prompt */
	if (isatty(fd))
		printf("> ");
	fflush(stdout);

	/* read buffer */
	nread = read(fd, buf, 1023);
	if (nread == -1)
	{
		perror("Error");
		goto loop;
	}
	/* handle end of file */
	if (feof(stdin))
		return (0);
	/* set buffer end */
	buf[nread] = 0;

	token = lex(buf);
	while (token)
	{
		print_token(token);
		token_free(token);
		token = lex(NULL);
	}

	if (isatty(fd))
		goto loop;
	return (0);
}
