
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
	char	*executable_path;

	if (ft_setup_redirections(current->redirs) == -1)
		exit(1);
	if (!current->args || !current->args[0])
		exit(0);
	if (ft_is_builtin(current->args[0]))
		exit(ft_exec_builtin(shell, current->args));
	executable_path = ft_find_executable(current->args[0], shell->env);
	if (!executable_path)
	{
		ft_print_command_error(current->args[0], CMD_NOT_FOUND);
		exit(MS_CMD_NOT_FOUND);
	}
	execve(executable_path, current->args, shell->env);
	perror(current->args[0]);
	free(executable_path);
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
	ft_wait_for_processes(shell, last_pid);
}
