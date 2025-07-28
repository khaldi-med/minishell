
#include "../../include/minishell.h"

static void	ft_handle_builtin(t_shell *shell, t_cmd *cmd)
{
	if (cmd->redirs)
		shell->exit_status = ft_exec_built_with_redir(shell, cmd);
	else
		shell->exit_status = ft_exec_builtin(shell, cmd->args);
}

static void	ft_handle_external_cmd(t_shell *shell, t_cmd *cmd)
{
	pid_t	pid;

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

void	ft_wait_for_processes(t_shell *shell, pid_t last_pid)
{
	int	status;

	while (wait(&status) > 0)
	{
		if (waitpid(last_pid, &status, WNOHANG) == last_pid)
		{
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else
				shell->exit_status = 128 + WTERMSIG(status);
		}
	}
}

void	ft_exec_sing_cmd(t_shell *shell, t_cmd *cmd)
{
	if (!cmd->args || !cmd->args[0])
	{
		ft_handle_no_args(shell, cmd);
		return ;
	}
	if (ft_is_builtin(cmd->args[0]))
	{
		ft_handle_builtin(shell, cmd);
		return ;
	}
	ft_handle_external_cmd(shell, cmd);
}

void	ft_exec_cmds(t_shell *shell, t_cmd *cmds)
{
	if (!cmds || !cmds->args)
		return ;
	ft_preprocess_heredocs(cmds);
	if (!cmds->next)
		ft_exec_sing_cmd(shell, cmds);
	else
		ft_exec_pipe(shell, cmds);
}
