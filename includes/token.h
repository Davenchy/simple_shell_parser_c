#ifndef __TOKEN_H__
#define __TOKEN_H__
#include <stddef.h>

/**
 * struct token_s - structure to hold the token info
 * @text: pointer to the allocated memory for the token text
 * @size: the size of the allocated memory
 * @type: the token type
 */
typedef struct token_s
{
	char *text;
	size_t size;
	char type;
} token_t;

token_t *token_create(char *text, size_t size, char type);
token_t *token_typed(char type);
void token_free(token_t *token);

#endif
