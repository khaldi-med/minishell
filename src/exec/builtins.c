#include "../../include/minishell.h"

int	ft_is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	ft_exec_builtin(t_shell *shell, char **args)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (ft_builtin_echo(args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_builtin_cd(shell, args));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_builtin_pwd());
	if (ft_strcmp(args[0], "export") == 0)
		return (ft_builtin_export(shell, args));
	if (ft_strcmp(args[0], "unset") == 0)
		return (ft_builtin_unset(shell, args));
	if (ft_strcmp(args[0], "env") == 0)
		return (ft_builtin_env(shell));
	if (ft_strcmp(args[0], "exit") == 0)
		return (ft_builtin_exit(shell, args));
	return (1);
}
