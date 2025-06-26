#include "../../include/minishell.h"

int	ft_is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
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

int	ft_builtin_exit(t_shell *shell, char **args)
{
	long	exit_code;

	ft_putstr_fd("exit\n", 2);
	if (args[1])
	{
		if (!ft_is_valid_number(args[1]))
		{
			ft_print_command_error("exit", NUMERIC_REQUIRED);
			ft_cleanup_shell(shell);
			rl_clear_history();
			exit(2);
		}
		if (args[2])
		{
			ft_print_command_error("exit", TOO_MANY_ARGS);
			return (1);
		}
		exit_code = ft_atoi(args[1]);
	}
	else
		exit_code = shell->exit_status;
	ft_cleanup_shell(shell);
	rl_clear_history();
	exit((unsigned char)exit_code);
}
