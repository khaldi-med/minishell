/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammed                                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-04                              #+#    #+#             */
/*   Updated: 2025-05-05                             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Check if a string is a valid identifier
 */
int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/**
 * Free old environment array and allocate a new one
 */
int	update_env_array(t_shell *shell)
{
	int		i;
	t_env	*curr;
	
	if (shell->env_array)
	{
		i = 0;
		while (shell->env_array[i])
			free(shell->env_array[i++]);
		free(shell->env_array);
	}
	i = 0;
	curr = shell->env_list;
	while (curr && ++i)
		curr = curr->next;
	shell->env_array = malloc(sizeof(char *) * (i + 1));
	if (!shell->env_array)
		return (ERROR);
	shell->env_array[i] = NULL;
	return (SUCCESS);
}

/**
 * Count number of environment variables
 */
int	count_env_vars(t_env *env_list)
{
	int		count;
	t_env	*curr;

	count = 0;
	curr = env_list;
	while (curr && ++count)
		curr = curr->next;
	return (count);
}

/**
 * Fill the sorted array with environment keys
 */
int	fill_sorted_array(t_env *env_list, char **sorted)
{
	int		i;
	t_env	*curr;

	i = 0;
	curr = env_list;
	while (curr)
	{
		sorted[i++] = curr->key;
		curr = curr->next;
	}
	sorted[i] = NULL;
	return (i);
}

/**
 * Print the sorted environment variables
 */
void	print_sorted_env(t_env *env_list, char **sorted, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		printf("declare -x %s=\"%s\"\n", sorted[i], 
			get_env_value(env_list, sorted[i]));
		i++;
	}
}

/**
 * Sort and print environment variables for export
 */
void	sort_and_print_export(t_env *env_list)
{
	char	**sorted;
	int		count;

	count = count_env_vars(env_list);
	sorted = malloc(sizeof(char *) * (count + 1));
	if (!sorted)
		return ;
	
	fill_sorted_array(env_list, sorted);
	print_sorted_env(env_list, sorted, count);
	
	free(sorted);
}

/**
 * Process a single export argument
 */
void	process_export_arg(t_shell *shell, char *arg)
{
	char	*key;
	char	*value;
	char	*equal_sign;

	if (!is_valid_identifier(arg))
	{
		fprintf(stderr, "export: not valid identifier: %s\n", arg);
		return;
	}
	
	equal_sign = strchr(arg, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		key = arg;
		value = equal_sign + 1;
		update_env(shell->env_list, key, value);
		*equal_sign = '=';
	}
}

/**
 * Export command: set environment variables
 */
int	builtin_export(t_shell *shell, t_command *cmd)
{
	int	i;

	if (!cmd->args[1])
	{
		sort_and_print_export(shell->env_list);
		return (SUCCESS);
	}
	
	i = 1;
	while (cmd->args[i])
	{
		process_export_arg(shell, cmd->args[i]);
		i++;
	}
	
	return (update_env_array(shell));
}

/**
 * Remove a single environment variable
 */
void	remove_env_var(t_shell *shell, char *key)
{
	t_env	*curr;
	t_env	*prev;

	if (!is_valid_identifier(key))
	{
		fprintf(stderr, "unset: not valid identifier: %s\n", key);
		return;
	}
	
	curr = shell->env_list;
	prev = NULL;
	while (curr)
	{
		if (strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				shell->env_list = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			break;
		}
		prev = curr;
		curr = curr->next;
	}
}

/**
 * Unset command: remove environment variables
 */
int	builtin_unset(t_shell *shell, t_command *cmd)
{
	int	i;

	if (!cmd->args[1])
		return (SUCCESS);
	
	i = 1;
	while (cmd->args[i])
	{
		remove_env_var(shell, cmd->args[i]);
		i++;
	}
	
	return (update_env_array(shell));
}
