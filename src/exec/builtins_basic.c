
#include "../../include/minishell.h"

int	ft_builtin_pwd(char **args)
{
	char	cwd[MAX_PATH];

	if (args[1])
	{
		ft_putstr_fd("bash: pwd: too many arguments", 2);
		ft_putendl_fd("", 2);
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		ft_print_file_error("pwd");
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	return (0);
}

int	ft_builtin_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		ft_putendl_fd(shell->env[i], 1);
		i++;
	}
	return (0);
}

int	ft_builtin_unset(t_shell *shell, char **args)
{
	int	i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		ft_unset_env_value(shell, args[i]);
		i++;
	}
	return (0);
}
