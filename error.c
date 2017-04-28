#include <stdio.h>
#include <stdlib.h>

void unix_error(const char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}
