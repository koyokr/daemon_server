#include "communicate.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>

static int client(int fd)
{
	int ret;

	ret = communicate(fd);
	close(fd);

	exit(0);
}

static void accept_loop(int fd)
{
	int cfd;
	struct sockaddr_in sin;
	socklen_t socklen;

	while (1) {
		cfd = accept(fd, (struct sockaddr *)&sin, &socklen);
		if (cfd == -1)
			continue;

		if (!fork()) {
			close(fd);
			client(cfd);
		}
		close(cfd);
	}
}

static int open_server(int domain, int port)
{
	int fd;
	int y = 1;
	struct sockaddr_in sin = {
		.sin_family      = domain,
		.sin_port        = htons(port),
		.sin_addr.s_addr = htonl(INADDR_ANY)
	};

	fd = socket(domain, SOCK_STREAM, 0);
	if (fd == -1)
		unix_error("socket");

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y)) == -1)
		unix_error("setsockopt");

	if (bind(fd, (struct sockaddr *)&sin, sizeof(sin)) == -1)
		unix_error("bind");

	if (listen(fd, 5) == -1)
		unix_error("listen");

	return fd;
}

int main(int argc, char *argv[])
{
	int fd;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		return 1;
	}

	if (daemon(0, 0) == -1)
		unix_error("daemon");

	signal(SIGCHLD, SIG_IGN);

	fd = open_server(AF_INET, atoi(argv[1]));
	accept_loop(fd);

	return 0;
}
