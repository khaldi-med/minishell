#include "../include/minishell.h"
#include <readline/readline.h>
#include <stdio.h>

int	main(void)
{
	char	*input;

	input = readline("Minishell$> ");
	printf("%s", input);
	return (0);
}
