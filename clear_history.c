#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	rl_clear_history();
	printf("Command history cleared\n");
	return (0);
}
