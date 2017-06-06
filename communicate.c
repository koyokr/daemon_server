#include "communicate.h"
#include "error.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFLEN 1024

static ssize_t read_wrap(int fd, char *buf, size_t count)
{
	int len;
	
	len = read(fd, buf, count);
	if (len == -1)
		unix_error("read");
	return len;
}

static ssize_t write_wrap(int fd, char const *buf, size_t count)
{
	int len;
	
	len = write(fd, buf, count);
	if (len == -1)
		unix_error("write");
	return len;
}

int communicate(int fd)
{
	char buf[BUFLEN];

	write_wrap(fd, "Hello, world!\n", sizeof("Hello, world!\n"));
	return 0;
}
