#include "communicate.h"
#include "error.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

static ssize_t read_wrap(int fd, char *buf)
{
	int len;
	
	len = read(fd, buf, BUFSIZ);
	if (len == -1)
		unix_error("read");

	return len;
}

static ssize_t write_wrap(int fd, char const *buf)
{
	int len;
	
	len = write(fd, buf, strlen(buf));
	if (len == -1)
		unix_error("write");

	return len;
}

int communicate(int fd)
{
	char buf[BUFSIZ] = "Hello, world!\n";
	int len;

	len = write_wrap(fd, buf);

	return 0;
}
