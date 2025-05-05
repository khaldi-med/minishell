
#include "../minishell.h"

/**
 * Extract key and value from an environment string
 * Format: KEY=VALUE
 */
int	extract_env_pair(char *env_str, char **key, char **value)
{
	char	*equal_sign;

	if (!env_str || !key || !value)
		return (ERROR);
	equal_sign = strchr(env_str, '=');
	if (!equal_sign)
		return (ERROR);
	
	/* Calculate lengths and allocate memory */
	*key = malloc(equal_sign - env_str + 1);
	if (!*key)
		return (ERROR);
	
	strncpy(*key, env_str, equal_sign - env_str);
	(*key)[equal_sign - env_str] = '\0';
	
	*value = strdup(equal_sign + 1);
	if (!*value)
	{
		free(*key);
		return (ERROR);
	}
	return (SUCCESS);
}

/**
 * Process a single environment variable and add it to the list
 */
int	process_env_var(t_env **env_list, char *env_str)
{
	char	*key;
	char	*value;
	t_env	*new_node;
	int		result;

	if (!env_list || !env_str)
		return (ERROR);
	
	result = extract_env_pair(env_str, &key, &value);
	if (result == ERROR)
		return (ERROR);
	
	/* Create and add new environment variable node */
	new_node = add_env(*env_list, key, value);
	if (!new_node)
	{
		free(key);
		free(value);
		return (ERROR);
	}
	
	if (!*env_list)
		*env_list = new_node;
	
	free(key);
	free(value);
	return (SUCCESS);
}

/**
 * Initialize environment variables list from the environment array
 */
t_env	*init_env(char **env)
{
	t_env	*env_list;
	int		i;

	if (!env)
		return (NULL);
		
	env_list = NULL;
	i = 0;
	
	while (env[i])
	{
		if (process_env_var(&env_list, env[i]) == ERROR)
		{
			/* Clean up on error */
			if (env_list)
				free_env_list(env_list);
			return (NULL);
		}
		i++;
	}
	
	return (env_list);
}

