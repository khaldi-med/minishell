#include "minishell.h"
// #include <cstdio.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline(PROMPT);
		if (input && *input)
		{
			//parse(input)
			//execute(input)
			add_history(input);
			printf("%s\n", input);
		}
		else
		{
			// 			stderr("ERROR!");
			return (1);
		}
	}
	free(input);
	return (0);
}
