#include "minishell.h"
#include <cstdio>
#include <readline/readline.h>
#include <stdio.h>

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline(PROMPT);
		if (input)
		{
			//parse(input)
			//execute(input)
			printf("%s\n", input);
		}
	}
	free(input);
	return (0);
}
