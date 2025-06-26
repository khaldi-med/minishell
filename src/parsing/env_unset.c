#include "../../include/minishell.h"

int	ft_count_remainig_env(char **env, char *key)
{
	int	i;
	int	key_len;
	int	count;

	i = 0;
	count = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		if (!(ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '='))
			count++;
		i++;
	}
	return (count);
}

static void	ft_rebuild_env_arry(t_shell *shell, char *key, int count)
{
	char	**new_env;
	int		i;
	int		j;
	int		key_len;

	key_len = ft_strlen(key);
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return ;
	i = 0;
	j = 0;
	while (shell->env[i])
	{
		if (!(ft_strncmp(shell->env[i], key, key_len) == 0 &&
				shell->env[i][key_len] == '='))
			new_env[j++] = shell->env[i];
		else
			free(shell->env[i]);
		i++;
	}
	new_env[j] = NULL;
	free(shell->env);
	shell->env = new_env;
}

void	ft_unset_env_value(t_shell *shell, char *key)
{
	int	count;

	if (!shell || !key || !shell->env)
		return ;
	count = ft_count_remainig_env(shell->env, key);
	ft_rebuild_env_arry(shell, key, count);
}
