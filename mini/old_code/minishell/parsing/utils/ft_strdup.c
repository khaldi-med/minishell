#include "../../inc/minishell.h"

char	*ft_strdup(const char *cmd)
{
	int		i;
	char	*ret;

	if (!cmd)
		return (NULL);
	
	i = 0;
	ret = malloc(sizeof(char) * ft_strlen(cmd) + 1);
	if (!ret)
		return (NULL);
	
	while (cmd[i])
	{
		ret[i] = cmd[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}
