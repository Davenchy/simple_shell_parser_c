#include "includes/lexer.h"
#include "includes/token.h"
#include <string.h>

size_t read_text(char *str)
{
	char *begin = str;

	if (!str)
		return 0;
	for (; *str; str++)
		if (*str == '\\' && *(str + 1) == '"')
			str += 2;
		else if (*str == '"')
			break;
	return (str - begin);

}

token_t *lex(char *str)
{
	static char *ptr;
	size_t size;

	if (str)
		ptr = str;
begin:
	if (!ptr && *ptr)
		return (NULL); /* return NULL to end stream if no more chars */
	ptr += strspn(ptr, WHITESPACE); /* ignore extra whitespace */
	if (*ptr == '\\')
	{
		ptr++;
		goto string;
	}
	switch (*ptr)
	{
		case 0:
			return (NULL); /* return NULL to end stream if no more chars */
		case ';':
			ptr++;
			return (token_typed(T_CMD)); /* return command end */
		case '$':
			ptr += size = strcspn(ptr + 1, TOKEN_DELIM) + 1; /* read var name */
			if (size == 1)
				return (token_create(ptr, 0, T_ERR));  /* else return error */
			return (token_create(ptr - size + 1, size - 1, T_VAR)); /* var name */
		case '&':
			if (*(ptr + 1) != '&')
				return (token_create(ptr - 1, 0, T_ERR));
			ptr += 2;
			return (token_typed(T_AND));
		case '|':
			ptr += size = ptr[2] == '|' ? 0 : ptr[1] == '|' ? 2 : 1;
			if (size)
				return (token_typed(size == 1 ? T_PIPE : T_OR));
			return (token_create(ptr, 0, T_ERR));
		case '"':
			size = read_text(ptr + 1);
			ptr += size + 2;
			return (token_create(ptr - size - 1, size, T_TEXT));
		default:
string:
			ptr += size = strcspn(ptr, TOKEN_DELIM); /* read text */
			if (size)
				return (token_create(ptr - size, size, T_TEXT));
	}
	goto begin; /* no token then repeat */
}
