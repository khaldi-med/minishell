
#include "../../include/minishell.h"

void	ft_print_error(char *prefix, char *message)
{
	write(STDERR_FILENO, "minishell: ", 11);
	if (prefix)
	{
		write(STDERR_FILENO, prefix, ft_strlen(prefix));
		write(STDERR_FILENO, ": ", 2);
	}
	if (message)
		write(STDERR_FILENO, message, ft_strlen(message));
	write(STDERR_FILENO, "\n", 1);
}

void	ft_print_command_error(char *command, char *message)
{
	write(STDERR_FILENO, "minishell: ", 11);
	if (command)
	{
		write(STDERR_FILENO, command, ft_strlen(command));
		write(STDERR_FILENO, ": ", 2);
	}
	if (message)
		write(STDERR_FILENO, message, ft_strlen(message));
	write(STDERR_FILENO, "\n", 1);
}

void	ft_print_file_error(char *filename)
{
	write(STDERR_FILENO, "minishell: ", 11);
	if (filename)
	{
		write(STDERR_FILENO, filename, ft_strlen(filename));
		write(STDERR_FILENO, ": ", 2);
	}
	write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
	write(STDERR_FILENO, "\n", 1);
}

void	ft_print_syntax_error(char *token)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, SYNTAX_ERROR, ft_strlen(SYNTAX_ERROR));
	write(STDERR_FILENO, " `", 2);
	if (token)
		write(STDERR_FILENO, token, ft_strlen(token));
	else
		write(STDERR_FILENO, "newline", 7);
	write(STDERR_FILENO, "'\n", 2);
}

int	ft_handle_export_error(char *identifier)
{
	write(STDERR_FILENO, "minishell: export: `", 20);
	if (identifier)
		write(STDERR_FILENO, identifier, ft_strlen(identifier));
	write(STDERR_FILENO, "': ", 3);
	write(STDERR_FILENO, INVALID_IDENTIFIER, ft_strlen(INVALID_IDENTIFIER));
	write(STDERR_FILENO, "\n", 1);
	return (1);
}
