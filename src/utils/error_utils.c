/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammed <mohammed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:04:00 by mohammed          #+#    #+#             */
/*   Updated: 2025/06/24 00:04:24 by mohkhald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Print pipe syntax error
 * Format: minishell: syntax error near unexpected token `|'
 */
void	ft_print_pipe_error(void)
{
	ft_print_syntax_error("|");
}

/**
 * Handle cd command errors with proper error messages
 */
int	ft_handle_cd_error(char *path)
{
	if (errno == ENOENT)
		ft_print_command_error("cd", MSG_NO_SUCH_FILE);
	else if (errno == ENOTDIR)
		ft_print_command_error("cd", MSG_NOT_A_DIRECTORY);
	else if (errno == EACCES)
		ft_print_command_error("cd", MSG_PERMISSION_DENIED);
	else
		ft_print_file_error(path);
	return (MS_GENERAL_ERROR);
}

/**
 * Handle redirection errors
 */
int	ft_handle_redirection_error(char *filename, int error_type)
{
	(void)error_type;
	ft_print_file_error(filename);
	return (-1);
}

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
