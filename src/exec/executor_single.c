
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

void	ft_handle_no_args(t_shell *shell, t_cmd *cmd)
{
	int	saved_stdout;
	int	saved_stdin;

	if (cmd->redirs)
	{
		ft_save_fds(&saved_stdout, &saved_stdin);
		if (ft_setup_redirections(cmd->redirs) == -1)
			shell->exit_status = 1;
		else
			shell->exit_status = 0;
		ft_restore_fds(saved_stdout, saved_stdin);
	}
	else
		shell->exit_status = 0;
}

int	ft_exec_built_with_redir(t_shell *shell, t_cmd *cmd)
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

void	ft_wait_for_child(pid_t pid, t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}
