#include "minishell.h"
#include <time.h>

int	main(void)
{
	char	*cwd_buffer;

	cwd_buffer = getcwd(NULL, 0);
	if (cwd_buffer)
	{
		printf("%s\n", cwd_buffer);
	}
	else
	{
		perror("getcwd");
	}
	free(cwd_buffer);
	return (0);
}
