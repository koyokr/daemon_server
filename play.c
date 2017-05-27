#include "play.h"
#include "error.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

ssize_t read_wrap(int fd, char *buf)
{
	int len = read(fd, buf, BUFSIZ);
	if (len == -1)
		unix_error("read");

	return len;
}

ssize_t write_wrap(int fd, const char *buf)
{
	int len = write(fd, buf, strlen(buf));
	if (len == -1)
		unix_error("write");

	return len;
}

int play(int fd)
{
	char buf[BUFSIZ];

	return fd;
}
