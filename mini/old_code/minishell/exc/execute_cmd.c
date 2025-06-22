#include "../inc/minishell.h"

extern int	g_exit_status;

/*
 * Execute external command with proper signal handling and error checking
 */
static int	execute_external_command(t_bash *bash, t_cmd *cmd)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;

	if (!cmd->arguments || !cmd->arguments[0])
		return (127);
	
	cmd_path = find_command_path(bash, cmd->arguments[0]);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->arguments[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		return (1);
	}
	if (pid == 0)
	{
		setup_child_signals();
		if (apply_redirections(cmd) != SUCCESS)
			exit(1);
		execve(cmd_path, cmd->arguments, bash->env_array);
		perror("minishell");
		exit(1);
	}
	free(cmd_path);
	waitpid(pid, &status, 0);
	setup_parent_signals();
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

/*
 * Execute a single command (builtin or external)
 */
int	execute_command(t_bash *bash, t_cmd *cmd, int *exit_status)
{
	if (!cmd || !cmd->arguments || !cmd->arguments[0])
		return (ERROR);
	
	// Apply redirections first
	if (cmd->s_red && !apply_redirections(cmd))
	{
		*exit_status = 1;
		return (ERROR);
	}
	
	if (is_builtin(cmd->arguments[0]))
		*exit_status = execute_builtin(bash, cmd, exit_status);
	else
		*exit_status = execute_external_command(bash, cmd);
	
	// Restore redirections
	if (cmd->s_red)
		restore_redirections();
		
	return (SUCCESS);
}

/*
 * Execute single command or pipeline based on number of commands
 */
int	execute_commands(t_bash *bash)
{
	int	exit_status;

	if (bash->num_cmd == 1)
	{
		exit_status = 0;
		if (execute_command(bash, bash->s_cmd[0], &exit_status) == ERROR)
			return (1);
		return (exit_status);
	}
	else
		return (execute_pipeline(bash, &g_exit_status));
}
