#include "includes/token.h"
#include <stdlib.h>
#include <string.h>

/**
 * token_create - create token object
 * @text: the token's text if any
 * @size: the token's text's size if no text set it to zero
 * @type: the token's type
 * Description:
 * if the token is not a text token then set @text to NULL and @size to zero
 *
 * the @size is required because @text is duplicated using strndup function
 * so the text member must be freed
 * so it is recommended to use the token_free function
 *
 * @type is not affect the token object, it is just a piece of information
 * Return: the token object otherwise NULL on fail
 */
token_t *token_create(char *text, size_t size, char type)
{
	token_t *token;
	char has_text = text && size;

	token = malloc(sizeof(token_t));
	if (!token)
		return (NULL);

	token->text = has_text ? strndup(text, size) : NULL;
	if (has_text && !token->text)
	{
		free(token);
		return (NULL);
	}
	token->size = size;
	token->type = type;
	return (token);
}

/**
 * token_typed - alias for token_create to create typed tokens
 * @type: the token's type
 * Description:
 * This function is just an alias for token_create function used to define
 * a typed token which is a token with the type member set
 * and only the type member, the text and the size members are set to NULL, 0
 *
 * Return: token object otherwise NULL on fail
 */
token_t *token_typed(char type)
{
	return (token_create(NULL, 0, type));
}

/**
 * token_free - frees a token object
 * @token: pointer to the token object
 */
void token_free(token_t *token)
{
	if (!token)
		return;
	if (token->text)
		free(token->text);
	free(token);
}
