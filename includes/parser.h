#ifndef __PARSER_H__
#define __PARSER_H__
#define BUFSIZE 1024
#include "command.h"

cmd_t *getcmds(int fildes, unsigned int *count);

#endif
