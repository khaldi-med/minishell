
#include "../../include/minishell.h"

static void	ft_update_existing_env(char **env_var, char *key, char *value)
{
	char	*key_equals;
	char	*new_var;

	free(*env_var);
	key_equals = ft_strjoin(key, "=");
	if (!key_equals)
		return ;
	new_var = ft_strjoin(key_equals, value);
	free(key_equals);
	if (!new_var)
		return ;
	*env_var = new_var;
}

static char	*ft_create_env_var(char *key, char *value)
{
	char	*key_equals;
	char	*new_var;

	key_equals = ft_strjoin(key, "=");
	if (!key_equals)
		return (NULL);
	new_var = ft_strjoin(key_equals, value);
	free(key_equals);
	return (new_var);
}

static char	**ft_create_expanded_env(char **old_env, int count)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = old_env[i];
		i++;
	}
	return (new_env);
}

static void	ft_add_new_env(t_shell *shell, char *key, char *value, int count)
{
	char	**new_env;
	char	*new_var;

	new_env = ft_create_expanded_env(shell->env, count);
	if (!new_env)
		return ;
	new_var = ft_create_env_var(key, value);
	if (!new_var)
	{
		free(new_env);
		return ;
	}
	new_env[count] = new_var;
	new_env[count + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
}

void	ft_set_env_value(t_shell *shell, char *key, char *value)
{
	int	i;
	int	key_len;

	key_len = ft_strlen(key);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], key, key_len) == 0
			&& shell->env[i][key_len] == '=')
		{
			ft_update_existing_env(&shell->env[i], key, value);
			return ;
		}
		i++;
	}
	ft_add_new_env(shell, key, value, i);
}
