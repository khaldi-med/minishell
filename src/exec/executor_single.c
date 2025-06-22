#include "../../include/minishell.h"

static void	ft_save_fds(int *save_stdout, int *save_stdin)
{
	*save_stdout = dup(1);
	*save_stdin = dup(0);
}

static void	ft_restore_fds(int saved_stdout, int saved_stdin)
{
	dup2(saved_stdout, 1);
	dup2(saved_stdin, 0);
	close(saved_stdout);
	close(saved_stdin);
}

static int	ft_exec_built_with_redir(t_shell *shell, t_cmd *cmd)
{
	int	saved_stdout;
	int	saved_stdin;
	int	ret;

	ft_save_fds(&saved_stdout, &saved_stdin);
	if (ft_setup_redirections(cmd->redirs) == -1)
	{
		ft_restore_fds(saved_stdout, saved_stdin);
		return (1);
	}
	ret = ft_exec_builtin(shell, cmd->args);
	ft_restore_fds(saved_stdout, saved_stdin);
	return (ret);
}

static void	ft_wait_for_child(pid_t pid, t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

void	ft_exec_sing_cmd(t_shell *shell, t_cmd *cmd)
{
	pid_t	pid;

	if (!cmd->args || !cmd->args[0])
	{
		shell->exit_status = 0;
		return ;
	}
	if (ft_is_builtin(cmd->args[0]))
	{
		if (cmd->redirs)
			shell->exit_status = ft_exec_built_with_redir(shell, cmd);
		else
			shell->exit_status = ft_exec_builtin(shell, cmd->args);
		return ;
	}
	pid = fork();
	if (pid == 0)
		ft_exec_external_cmd(cmd, shell->env);
	else if (pid > 0)
		ft_wait_for_child(pid, shell);
	else
	{
		perror("fork");
		shell->exit_status = 0;
	}
}
