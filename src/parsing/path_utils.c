
#include "../../include/minishell.h"

char	*ft_get_env_value(char **env, char *key)
{
	int		i;
	int		key_len;
	char	*value;

	i = 0;
	if (!key || !env)
		return (NULL);
	key_len = ft_strlen(key);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
		{
			value = env[i] + key_len + 1;
			return (ft_strdup(value));
		}
		i++;
	}
	return (NULL);
}

char	**ft_get_path_dirs(char **env)
{
	char	*path_env;
	char	**dirs;

	path_env = ft_get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	free(path_env);
	return (dirs);
}

void	ft_free_path_dirs(char **path_dirs)
{
	int	i;

	if (!path_dirs)
		return ;
	i = 0;
	while (path_dirs[i])
	{
		free(path_dirs[i]);
		i++;
	}
	free(path_dirs);
}
