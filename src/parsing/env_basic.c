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

char	**ft_copy_env(char **env)
{
	int		i;
	int		count;
	char	**new_env;

	i = 0;
	count = 0;
	while (env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	while (i < count)
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

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

static void	ft_add_new_env(t_shell *shell, char *key, char *value, int count)
{
	char	**new_env;
	char	*key_equals;
	char	*new_var;
	int		i;

	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return ;
	i = 0;
	while (i < count)
	{
		new_env[i] = shell->env[i];
		i++;
	}
	key_equals = ft_strjoin(key, "=");
	if (!key_equals)
	{
		free(new_env);
		return ;
	}
	new_var = ft_strjoin(key_equals, value);
	free(key_equals);
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
		if (ft_strncmp(shell->env[i], key, key_len) == 0 &&
			shell->env[i][key_len] == '=')
		{
			ft_update_existing_env(&shell->env[i], key, value);
			return ;
		}
		i++;
	}
	ft_add_new_env(shell, key, value, i);
}
