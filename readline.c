#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	char	*line;

	line = readline("Enter your name: ");
	if (line == NULL)
	{
		printf("Error reading line\n");
		return (1);
	}
	printf("Hello, %s!\n", line);
	free(line);
	return (0);
}
