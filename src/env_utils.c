
#include "../minishell.h"

/**
 * Initialize a new env node with key and value
 */
t_env	*create_env_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = strdup(key);
	new_node->value = strdup(value);
	new_node->next = NULL;
	if (!new_node->key || !new_node->value)
	{
		if (new_node->key)
			free(new_node->key);
		if (new_node->value)
			free(new_node->value);
		free(new_node);
		return (NULL);
	}
	return (new_node);
}

/**
 * Add a new environment variable to the list
 */
t_env	*add_env(t_env *env_list, char *key, char *value)
{
	t_env	*new_node;
	t_env	*current;

	new_node = create_env_node(key, value);
	if (!new_node)
		return (NULL);
	
	/* If list is empty, return the new node */
	if (!env_list)
		return (new_node);
	
	/* Add to the end of the list */
	current = env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
	return (new_node);
}

/**
 * Get the value of an environment variable
 */
char	*get_env_value(t_env *env_list, char *key)
{
	t_env	*current;

	if (!env_list || !key)
		return (NULL);
	current = env_list;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

/**
 * Update or add an environment variable
 */
int	update_env(t_env *env_list, char *key, char *value)
{
	t_env	*current;

	if (!env_list || !key || !value)
		return (ERROR);
	current = env_list;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = strdup(value);
			if (!current->value)
				return (ERROR);
			return (SUCCESS);
		}
		current = current->next;
	}
	/* Key not found, add new entry */
	if (!add_env(env_list, key, value))
		return (ERROR);
	return (SUCCESS);
}

/**
 * Helper function to allocate memory for env array
 */
char	**allocate_env_array(int count)
{
	char	**env_array;
	
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	env_array[count] = NULL;
	return (env_array);
}

/**
 * Create a single environment string (KEY=VALUE)
 */
char	*create_env_string(t_env *env_node)
{
	char	*temp;
	
	temp = malloc(strlen(env_node->key) + strlen(env_node->value) + 2);
	if (!temp)
		return (NULL);
	sprintf(temp, "%s=%s", env_node->key, env_node->value);
	return (temp);
}

/**
 * Convert environment list to string array (for execve)
 */
char	**env_list_to_array(t_env *env_list)
{
	t_env	*current;
	char	**env_array;
	int		count;
	int		i;

	count = 0;
	current = env_list;
	while (current && ++count)
		current = current->next;
	
	env_array = allocate_env_array(count);
	if (!env_array)
		return (NULL);
	
	current = env_list;
	i = 0;
	while (current)
	{
		env_array[i] = create_env_string(current);
		if (!env_array[i])
		{
			i--;
			while (i >= 0)
			{
				free(env_array[i]);
				i--;
			}
			free(env_array);
			return (NULL);
		}
		i++;
		current = current->next;
	}
	return (env_array);
}

/**
 * Free the environment variables linked list
 */
void	free_env_list(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env;
		env = env->next;
		if (temp->key)
			free(temp->key);
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}

