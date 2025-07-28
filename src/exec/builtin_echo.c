
#include "../../include/minishell.h"

static int	is_valid_n_flag(char *arg)
{
	int	j;

	if (arg[0] != '-' || arg[1] == '\0')
		return (0);
	j = 1;
	while (arg[j])
	{
		if (arg[j] != 'n')
			return (0);
		j++;
	}
	return (j > 1);
}

static void	process_n_flags(char **args, int *start_index, int *print_newline)
{
	*start_index = 1;
	*print_newline = 1;
	while (args[*start_index])
	{
		if (is_valid_n_flag(args[*start_index]))
		{
			*print_newline = 0;
			(*start_index)++;
		}
		else
			break ;
	}
}

int	ft_builtin_echo(char **args)
{
	int	i;
	int	newline;
	int	first_arg;

	process_n_flags(args, &i, &newline);
	first_arg = 1;
	while (args[i])
	{
		if (!first_arg)
			ft_putstr_fd(" ", 1);
		ft_putstr_fd(args[i], 1);
		first_arg = 0;
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", 1);
	return (0);
}
