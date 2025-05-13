#include "minishell.h"

int	main(void)
{
	if (access("/bin/ls", R_OK) == -1)
	{
		printf("Permission denied\n");
	}
	else
	{
		printf("Readable\n");
	}
	return (0);
}
