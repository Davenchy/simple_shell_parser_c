#ifndef __LEXER_H__
#define __LEXER_H__
#include "token.h"

#define WHITESPACE " \t\r\n"
#define TOKEN_DELIM "$;|&\" \t\r\n"

#define T_ERR 0
#define T_CMD 1
#define T_TEXT 2
#define T_VAR 3
#define T_AND 4
#define T_OR 5
#define T_PIPE 6

token_t *lex(char *str);

#endif
