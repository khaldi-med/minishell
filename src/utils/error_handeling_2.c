
#include "../../include/minishell.h"

void	ft_print_exit_error(char *arg, char *message)
{
	write(STDERR_FILENO, "minishell: exit: ", 17);
	if (arg)
	{
		write(STDERR_FILENO, arg, ft_strlen(arg));
		write(STDERR_FILENO, ": ", 2);
	}
	if (message)
		write(STDERR_FILENO, message, ft_strlen(message));
	write(STDERR_FILENO, "\n", 1);
}

int	ft_validate_input(t_shell *shell, char *input)
{
	char	unclosed_quote;

	unclosed_quote = ft_check_unclosed_quotes(input);
	if (unclosed_quote)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(UNCLOSED_QUOTE, STDERR_FILENO);
		ft_putchar_fd(' ', STDERR_FILENO);
		ft_putchar_fd(unclosed_quote, STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
		shell->exit_status = SYNTAX_ERROR;
		return (0);
	}
	return (1);
}
