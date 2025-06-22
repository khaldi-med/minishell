#include "../inc/minishell.h"

/*
 * Execute a child process in the pipeline
 */
static void	execute_pipe_child(t_bash *bash, int i, int *prev_pipe,
	int *curr_pipe, int *exit_status)
{
	setup_child_signals();
	if (i > 0)
	{
		if (dup2(prev_pipe[0], STDIN_FILENO) == -1)
			exit(1);
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (i < bash->num_cmd - 1)
	{
		close(curr_pipe[0]);
		if (dup2(curr_pipe[1], STDOUT_FILENO) == -1)
			exit(1);
		close(curr_pipe[1]);
	}
	if (execute_command(bash, bash->s_cmd[i], exit_status) == ERROR)
		exit(1);
	exit(*exit_status);
}

/*
 * Set up pipe for the next command in pipeline
 */
static int	setup_pipe(int *pipe_fd)
{
	if (pipe(pipe_fd) == -1)
	{
		ft_putstr_fd("minishell: pipe error\n", 2);
		return (ERROR);
	}
	return (SUCCESS);
}

/*
 * Clean up pipe file descriptors in parent process
 */
static void	cleanup_pipes(int *prev_pipe, int *curr_pipe, int i, int num_cmd)
{
	if (i > 0)
	{
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (i < num_cmd - 1)
	{
		prev_pipe[0] = curr_pipe[0];
		prev_pipe[1] = curr_pipe[1];
	}
}

/*
 * Execute a pipeline of commands with proper signal handling
 */
int	execute_pipeline(t_bash *bash, int *exit_status)
{
	int		i;
	int		prev_pipe[2];
	int		curr_pipe[2];
	pid_t	*pids;
	int		status;

	pids = malloc(sizeof(pid_t) * bash->num_cmd);
	if (!pids)
		return (1);
	i = -1;
	while (++i < bash->num_cmd)
	{
		if (i < bash->num_cmd - 1 && setup_pipe(curr_pipe) == ERROR)
		{
			free(pids);
			return (1);
		}
		pids[i] = fork();
		if (pids[i] == -1)
		{
			free(pids);
			return (1);
		}
		if (pids[i] == 0)
			execute_pipe_child(bash, i, prev_pipe, curr_pipe, exit_status);
		cleanup_pipes(prev_pipe, curr_pipe, i, bash->num_cmd);
	}
	i = -1;
	while (++i < bash->num_cmd)
	{
		waitpid(pids[i], &status, 0);
		if (i == bash->num_cmd - 1)
		{
			if (WIFSIGNALED(status))
				*exit_status = 128 + WTERMSIG(status);
			else
				*exit_status = WEXITSTATUS(status);
		}
	}
	setup_parent_signals();
	free(pids);
	return (*exit_status);
}
