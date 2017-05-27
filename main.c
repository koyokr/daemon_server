#include "play.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>

void wait_client(int fd)
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
			play(cfd);
			close(cfd);

			exit(0);
		}
		close(cfd);
	}
}

void start_server(int port)
{
	int fd;
	struct sockaddr_in sin = {
		.sin_family      = AF_INET,
		.sin_port        = htons(port),
		.sin_addr.s_addr = htonl(INADDR_ANY)
	};
	int y = 1;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
		unix_error("socket");
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y)))
		unix_error("setsockopt");
	if (bind(fd, (struct sockaddr *)&sin, sizeof(sin)) == -1)
		unix_error("bind");
	if (listen(fd, 5) == -1)
		unix_error("listen");

	wait_client(fd);
}

void set_sigchld(void)
{
	struct sigaction sa = {
		.sa_handler = SIG_IGN,
		.sa_flags = 0
	};

	if (sigaction(SIGCHLD, &sa, NULL) == -1)
		unix_error("sigaction");
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("usage: %s <port>\n", argv[0]);
		return 1;
	}

	if (daemon(0, 0) == -1)
		unix_error("daemon");

	set_sigchld();
	start_server(atoi(argv[1]));
	return 0;
}
