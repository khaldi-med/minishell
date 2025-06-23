/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammed <mohammed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:09:00 by mohammed          #+#    #+#             */
/*   Updated: 2025/06/23 17:09:00 by mohammed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Check if string is a valid number for exit command
 */
static int	ft_is_valid_number(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i = 1;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/**
 * Handle exit command errors
 */
int	ft_handle_exit_error(char **args)
{
	if (args[1] && !ft_is_valid_number(args[1]))
	{
		ft_print_command_error("exit", MSG_NUMERIC_REQUIRED);
		return (2);
	}
	if (args[1] && args[2])
	{
		ft_print_command_error("exit", MSG_TOO_MANY_ARGS);
		return (1);
	}
	return (0);
}
