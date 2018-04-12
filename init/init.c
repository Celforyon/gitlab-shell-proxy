#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int pid = 0;

void sighandler(int sig) {
	printf("SIGNAL [%d] received\n", sig);
	kill(pid, sig);
	usleep(200000);
	kill(pid, 0);
	if(errno != ESRCH)
		kill(pid, SIGKILL);
}

int main(int argc, char **argv, char **env) {
	if(argc < 2) return 1;
	char **pargv = argv+1;
	int wstatus;

	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);
	signal(SIGQUIT, sighandler);
	signal(SIGUSR1, sighandler);
	signal(SIGUSR2, sighandler);

	switch(pid = fork()) {
	case -1:
		perror("fork");
		return EXIT_FAILURE;
	case 0:
		if(execve(pargv[0], pargv, env) == -1) {
			perror("exec");
			return EXIT_FAILURE;
		}
	default:
		if(waitpid(pid, &wstatus, 0) == -1) {
			perror("waitpid");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
