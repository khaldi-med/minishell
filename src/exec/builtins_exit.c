
#include "../../include/minishell.h"

static long	ft_get_exit_value(char *str)
{
	char	*trimmed;
	long	result;

	trimmed = ft_strtrim_whitespace(str);
	ft_atol(trimmed, &result);
	free(trimmed);
	return (result);
}

static void	ft_handle_invalid_arg(t_shell *shell, char *arg)
{
	ft_print_exit_error(arg, NUMERIC_REQUIRED);
	ft_cleanup_shell(shell);
	rl_clear_history();
	exit(2);
}

static int	ft_handle_too_many_args(void)
{
	ft_print_exit_error(NULL, TOO_MANY_ARGS);
	return (1);
}

static void	ft_perform_exit(t_shell *shell, long exit_code)
{
	ft_cleanup_shell(shell);
	rl_clear_history();
	exit((unsigned char)exit_code);
}

int	ft_builtin_exit(t_shell *shell, char **args)
{
	long	exit_code;

	ft_putstr_fd("exit\n", 2);
	if (args[1])
	{
		if (!ft_is_valid_exit_arg(args[1]))
			ft_handle_invalid_arg(shell, args[1]);
		if (args[2])
			return (ft_handle_too_many_args());
		exit_code = ft_get_exit_value(args[1]);
	}
	else
		exit_code = shell->exit_status;
	ft_perform_exit(shell, exit_code);
	return (0);
}
