#include "error.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define SERVER_PORT 2226

void play(int fd);

void wait_client(int fd)
{
	int cfd;
	struct sockaddr_in csin;
	socklen_t socklen = sizeof(struct sockaddr);

	while (1) {
		cfd = accept(fd, (struct sockaddr *)&csin, &socklen);
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
	struct sockaddr_in ssin = {
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
	if (bind(fd, (struct sockaddr *)&ssin, sizeof(struct sockaddr)) == -1)
		unix_error("bind");
	if (listen(fd, 10) == -1)
		unix_error("listen");

	wait_client(fd);
}

void waitpid_child(int signo)
{
	int stat;
	waitpid(-1, &stat, WNOHANG);
}

void set_sigchld(void)
{
	struct sigaction sa = { .sa_handler = waitpid_child };
	sigemptyset(&sa.sa_mask);

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

