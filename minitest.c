#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

int	main(void)
{
	char	input[MAX_INPUT_SIZE];
	char	*args[MAX_ARG_SIZE];
	int		status;
	char	*token;
	int		i;

	do
	{
		printf("my_shell> ");
		fgets(input, MAX_INPUT_SIZE, stdin);
		input[strcspn(input, "\n")] = 0; // Remove newline character
		// Tokenize the input
		token = strtok(input, " ");
		i = 0;
		while (token != NULL)
		{
			args[i++] = token;
			token = strtok(NULL, " ");
		}
		args[i] = NULL; // Null-terminate the array
		// Execute the command
		if (fork() == 0)
		{
			execvp(args[0], args);
			perror("exec failed");
			exit(EXIT_FAILURE);
		}
		else
		{
			wait(&status);
		}
	} while (1);
	return (0);
}
