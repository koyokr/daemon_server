#include "communicate.h"
#include "error.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFLEN 1024

static ssize_t read_wrap(int fd, char *buf)
{
	int len;
	
	len = read(fd, buf, BUFLEN);
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
	char buf[BUFLEN] = "Hello, world!\n";

	write_wrap(fd, buf);
	return 0;
}
