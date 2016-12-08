#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

int fd[3];
char ch;
int count = 0;

void sigchld_hander(int sig) { count += 1; close(fd[1]); }
void sigusr1_hander(int sig) { count += 10; }
void sigusr2_hander(int sig) { count += 100; ch = 'd'; }

int main(int argc, char **argv)
{
	int pid;

	signal(SIGCHLD, sigchld_hander);
	signal(SIGUSR1, sigusr1_hander);
	signal(SIGUSR2, sigusr2_hander);

	fd[0] = open("in.txt", O_RDWR, 0);
	fd[1] = open("in.txt", O_RDWR, 0);

	dup2(1, 0); dup2(2, 1);
	ch = 'a'; write(STDIN_FILENO, &ch, 1);
	ch = 'b'; write(STDOUT_FILENO, &ch, 1);
	ch = 'c'; write(STDERR_FILENO, &ch, 1);

	if ((pid = fork()) == 0) {
		kill(getppid(), SIGUSR1);
		kill(getppid(), SIGUSR1);

		fd[2] = open("out.txt", O_RDWR, 0);
		read(fd[0], &ch, 1); write(fd[2], &ch, 1);
		dup2(fd[0], fd[2]);
		read(fd[0], &ch, 1); write(fd[2], &ch, 1);
	} else {
		kill(pid, SIGUSR2);

		fd[2] = open("out.txt", O_RDWR, 0);
		while (waitpid(-1, NULL, 0) > 0) ;
		read(fd[0], &ch, 1); write(fd[0], &ch, 1);
		read(fd[2], &ch, 1); write(fd[2], &ch, 1);
	}

	exit(0);
}
