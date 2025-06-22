#include "../../include/minishell.h"

static int	ft_check_file_access(char *path, t_cmd *cmd)
{
	struct stat	st;

	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": Is a directory\n", 2);
			exit(126);
		}
		if (access(path, X_OK) == -1)
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(126);
		}
		return (1);
	}
	return (0);
}

static char	*ft_get_executable_path(t_cmd *cmd, char **env)
{
	char	*path;

	if (ft_strchr(cmd->args[0], '/'))
	{
		if (!ft_check_file_access(cmd->args[0], cmd))
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			exit(127);
		}
		path = cmd->args[0];
	}
	else
	{
		path = ft_find_executable(cmd->args[0], env);
		if (!path)
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(127);
		}
	}
	return (path);
}

void	ft_exec_external_cmd(t_cmd *cmd, char **env)
{
	char	*path;

	if (ft_setup_redirections(cmd->redirs) == -1)
		exit(1);
	path = ft_get_executable_path(cmd, env);
	execve(path, cmd->args, env);
	perror("execve");
	exit(126);
}
