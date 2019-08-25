#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void child1(pid_t pid); /*child process prototype*/
void child2(pid_t pid); /*child process prototype*/
void parent(pid_t pid); /*parent process prototype*/

void main(void)
{
	pid_t pid;
	pid = fork();
	if (pid == 0)
		child1(pid);
	else
	{
		pid = fork();
		if (pid == 0)
			child2(pid);
		else
			parent(pid);
	}
}

void child1(pid_t pid)
{
	printf("this line is from child1 pid %d and pid %d\n", getpid(), pid);
	printf("child 1 is done \n");
	execlp("/bin/ls", "ls", NULL);
}

void child2(pid_t pid)
{
	printf("this line is from child2 pid %d and pid %d\n", getpid(), pid);
	printf("child 2 is done \n");
	execlp("/bin/pwd", "pwd", NULL);
}

void parent(pid_t pid)
{
	printf("this is line is parent pid %d and pid %d\n", getpid(), pid);
	wait(NULL);
	wait(NULL);
	printf("Both child processes are finished \n");
}
