#include "minishell.h"
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
			printf("%s\n", input);
		}
	}
	return (0);
}
