// #include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	pid_t	pid;
	int		pid_num;

	pid_num = 0;
	pid = fork();
	if (pid == -1)
	{
		printf("Error forking\n");
		return (1);
	}
	if (pid == 0)
	{
		// Child process
		pid_num = getpid();
		printf("Child process pid_mun= %d\n", pid_num);
	}
	else
	{
		// Parent process
		pid_num = getppid();
		printf("Parent process pid_num= %d\n", pid_num);
	}
	return (0);
}
