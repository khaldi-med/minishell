#include "minishell.h"
#include <cstdio>
#include <readline/readline.h>
#include <time.h>

char	**ft_create_env_array(t_env *env_list)
{
	t_env	*current;
	char	**env_array;
	int		count;
	int		i;
	char	*tmp;

	if (!env_list)
		return (NULL);
	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	current = env_list;
	while (current)
	{
		tmp = malloc(ft_strlen(current->key) + ft_strlen(current->value) + 2);
		if (!tmp)
		{
			while (--i >= 0)
				free(env_array[i]);
			free(env_array);
			return (NULL);
		}
		env_array[i++] = tmp;
		current = current->next;
	}
	env_array = NULL;
	return (env_array);
}
