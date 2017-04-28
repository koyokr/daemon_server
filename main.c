#include "error.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

#define SERVER_PORT 2226

void play(int fd);

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

void create_server(void)
{
	int fd;
	struct sockaddr_in sin = {
		.sin_family      = AF_INET,
		.sin_port        = htons(SERVER_PORT),
		.sin_addr.s_addr = htonl(INADDR_ANY)
	};
	const int y = 1;

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

int main(void)
{
	if (daemon(0, 0) == -1)
		return 1;

	set_sigchld();
	create_server();
	return 0;
}
