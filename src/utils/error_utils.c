
#include "../../include/minishell.h"

void	ft_print_pipe_error(void)
{
	ft_print_syntax_error("|");
}

int	ft_handle_cd_error(char *path)
{
	if (errno == ENOENT)
		ft_print_command_error("cd", NO_SUCH_FILE);
	else if (errno == ENOTDIR)
		ft_print_command_error("cd", NOT_A_DIRECTORY);
	else if (errno == EACCES)
		ft_print_command_error("cd", PERMISSION_DENIED);
	else
		ft_print_file_error(path);
	return (GENERAL_ERROR);
}

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
		ft_print_command_error("exit", NUMERIC_REQUIRED);
		return (2);
	}
	if (args[1] && args[2])
	{
		ft_print_command_error("exit", TOO_MANY_ARGS);
		return (1);
	}
	return (0);
}
