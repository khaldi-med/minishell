/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammed                                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-04                              #+#    #+#             */
/*   Updated: 2025-05-05                             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Check if command is a built-in
 */
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (strcmp(cmd, "echo") == 0)
		return (1);
	if (strcmp(cmd, "cd") == 0)
		return (1);
	if (strcmp(cmd, "pwd") == 0)
		return (1);
	if (strcmp(cmd, "export") == 0)
		return (1);
	if (strcmp(cmd, "unset") == 0)
		return (1);
	if (strcmp(cmd, "env") == 0)
		return (1);
	if (strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

/**
 * Execute a built-in command
 */
int	execute_builtin(t_shell *shell, t_command *cmd)
{
	int	result;
	int	stdin_backup;
	int	stdout_backup;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (ERROR);
	
	/* Save original file descriptors */
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	
	/* Set up redirections for the built-in command */
	if (setup_redirections(cmd) == ERROR)
	{
		/* Handle redirection error */
		if (stdin_backup != -1)
			close(stdin_backup);
		if (stdout_backup != -1)
			close(stdout_backup);
		return (ERROR);
	}
	
	/* Execute the built-in command */
	if (strcmp(cmd->args[0], "echo") == 0)
		result = builtin_echo(cmd);
	else if (strcmp(cmd->args[0], "cd") == 0)
		result = builtin_cd(shell, cmd);
	else if (strcmp(cmd->args[0], "pwd") == 0)
		result = builtin_pwd();
	else if (strcmp(cmd->args[0], "export") == 0)
		result = builtin_export(shell, cmd);
	else if (strcmp(cmd->args[0], "unset") == 0)
		result = builtin_unset(shell, cmd);
	else if (strcmp(cmd->args[0], "env") == 0)
		result = builtin_env(shell);
	else if (strcmp(cmd->args[0], "exit") == 0)
		result = builtin_exit(shell, cmd);
	else
		result = ERROR;
	
	/* Restore original file descriptors */
	if (stdin_backup != -1)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
	}
	if (stdout_backup != -1)
	{
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
	}
	
	return (result);
}

/**
 * Set up input/output redirections
 */
int	setup_redirections(t_command *cmd)
{
	int	fd;

	if (cmd->in_file)
	{
		fd = open(cmd->in_file, O_RDONLY);
		if (fd < 0)
			return (ERROR);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cmd->out_file)
	{
		if (cmd->append)
			fd = open(cmd->out_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (ERROR);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (SUCCESS);
}

/**
 * Set up file descriptors for child process
 */
void	setup_child_fds(int in_fd, int *pipe_fd)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (pipe_fd && pipe_fd[1] != STDOUT_FILENO)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

/**
 * Execute command in child process
 */
void	execute_command_in_child(t_shell *shell, t_command *cmd)
{
	char	*path;

	path = find_command_path(cmd->args[0], shell->env_list);
	if (!path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmd->args[0]);
		exit(127);
	}
	execve(path, cmd->args, shell->env_array);
	perror("minishell");
	free(path);
	exit(1);
}

/**
 * Child process execution
 */
void	child_process(t_shell *shell, t_command *cmd, int in_fd, int *pipe_fd)
{
	setup_child_fds(in_fd, pipe_fd);
	setup_redirections(cmd);
	
	if (!shell->env_array)
		shell->env_array = env_list_to_array(shell->env_list);
		
	execute_command_in_child(shell, cmd);
}

/**
 * Set up and handle parent process operations
 */
void	parent_process(int in_fd, int *pipe_fd, pid_t pid, int *status)
{
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (pipe_fd)
		close(pipe_fd[1]);
	waitpid(pid, status, 0);
}

/**
 * Execute a single command
 */
int	execute_single_command(t_shell *shell, t_command *cmd, int in_fd, int *pipe_fd)
{
	pid_t	pid;
	int		status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (ERROR);
	if (is_builtin(cmd->args[0]))
		return (execute_builtin(shell, cmd));
	
	/* Make sure env_array is updated */
	if (!shell->env_array)
		shell->env_array = env_list_to_array(shell->env_list);
	
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (ERROR);
	}
	if (pid == 0)
		child_process(shell, cmd, in_fd, pipe_fd);
	
	parent_process(in_fd, pipe_fd, pid, &status);
	return (WEXITSTATUS(status));
}

/**
 * Initialize execution environment
 */
int	init_execution(t_shell *shell, int *in_fd, int *status)
{
	if (!shell || !shell->commands)
		return (ERROR);
	
	/* Ensure environment array is created */
	if (!shell->env_array)
		shell->env_array = env_list_to_array(shell->env_list);
	if (!shell->env_array)
		return (ERROR);
	
	*in_fd = STDIN_FILENO;
	*status = 0;
	return (SUCCESS);
}

/**
 * Set up pipe for next command
 */
int	setup_pipe_for_next_cmd(t_command *cmd, int pipe_fd[2])
{
	if (cmd->next && pipe(pipe_fd) < 0)
	{
		perror("minishell: pipe");
		return (ERROR);
	}
	return (SUCCESS);
}

/**
 * Execute all commands in the pipeline
 */
int	execute_commands(t_shell *shell)
{
	t_command	*cmd;
	int			pipe_fd[2];
	int			in_fd;
	int			status;

	if (init_execution(shell, &in_fd, &status) == ERROR)
		return (ERROR);
	
	cmd = shell->commands;
	while (cmd)
	{
		pipe_fd[1] = STDOUT_FILENO;
		if (setup_pipe_for_next_cmd(cmd, pipe_fd) == ERROR)
			return (ERROR);
			
		status = execute_single_command(shell, cmd, in_fd, 
			cmd->next ? pipe_fd : NULL);
			
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (cmd->next)
			in_fd = pipe_fd[0];
			
		cmd = cmd->next;
	}
	
	shell->exit_status = status;
	return (status);
}
