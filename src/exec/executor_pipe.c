#include "../../include/minishell.h"

static void	ft_setup_pip_redi(int prev_fd, int *pipfd, t_cmd *current)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, 0);
		close(prev_fd);
	}
	if (current->next)
	{
		dup2(pipfd[1], 1);
		close(pipfd[1]);
		close(pipfd[0]);
	}
}

static void	ft_exec_cmd_in_child(t_shell *shell, t_cmd *current)
{
	if (ft_setup_redirections(current->redirs) == -1)
		exit(1);
	if (ft_is_builtin(current->args[0]))
		exit(ft_exec_builtin(shell, current->args));
	execve(ft_find_executable(current->args[0], shell->env), current->args,
			shell->env);
	exit(1);
}

static pid_t	ft_creat_pip_poces(t_shell *shell, t_cmd *current, int prev_fd,
		int *pipfd)
{
	pid_t	pid;

	if (current->next)
		pipe(pipfd);
	pid = fork();
	if (pid == 0)
	{
		ft_setup_pip_redi(prev_fd, pipfd, current);
		ft_exec_cmd_in_child(shell, current);
	}
	return (pid);
}

static void	ft_clean_pip_fds(int prev_fd, int *pipfd, t_cmd *current)
{
	if (prev_fd != -1)
		close(prev_fd);
	if (current->next)
		close(pipfd[1]);
}

void	ft_exec_pipe(t_shell *shell, t_cmd *cmds)
{
	int		pipfd[2];
	int		prev_fd;
	t_cmd	*current;
	pid_t	last_pid;
	int		status;

	prev_fd = -1;
	current = cmds;
	last_pid = -1;
	while (current)
	{
		last_pid = ft_creat_pip_poces(shell, current, prev_fd, pipfd);
		ft_clean_pip_fds(prev_fd, pipfd, current);
		if (current->next)
			prev_fd = pipfd[0];
		current = current->next;
	}
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
