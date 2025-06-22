#include "../../include/minishell.h"

static int	ft_handle_inp_red(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (-1);
	dup2(fd, 0);
	close(fd);
	return (0);
}

static int	ft_handle_outp_red(char *file, int flags)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd == -1)
		return (-1);
	dup2(fd, 1);
	close(fd);
	return (0);
}

/* OLD CODE - COMMENTED OUT
int	ft_setup_redirections(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
	if (current->type == TOKEN_REDIR_IN)
		{
			if (ft_handle_inp_red(current->file) == -1)
				return (-1);
		}
	else if (current->type == TOKEN_REDIR_OUT)
		{
			if (ft_handle_outp_red(current->file,
					O_WRONLY | O_CREAT | O_APPEND) ==
				-1)
				return (-1);
		}
		else if (current->type == TOKEN_HEREDOC)
		{
			ft_setup_heredoc(current->file);
		}
		current = current->next;
	}
	return (0);
}
*/

// NEW IMPLEMENTATION - Fixed redirection handling
int	ft_setup_redirections(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN)
		{
			if (ft_handle_inp_red(current->file) == -1)
			{
				perror(current->file);
				return (-1);
			}
		}
		else if (current->type == TOKEN_REDIR_OUT)
		{
			// Use O_TRUNC for regular output redirection
			if (ft_handle_outp_red(current->file,
					O_WRONLY | O_CREAT | O_TRUNC) == -1)
			{
				perror(current->file);
				return (-1);
			}
		}
		else if (current->type == TOKEN_REDIR_APPEND)
		{
			// Use O_APPEND for append redirection
			if (ft_handle_outp_red(current->file,
					O_WRONLY | O_CREAT | O_APPEND) == -1)
			{
				perror(current->file);
				return (-1);
			}
		}
		else if (current->type == TOKEN_HEREDOC)
		{
			ft_setup_heredoc(current->file);
		}
		current = current->next;
	}
	return (0);
}

static void	ft_save_fds(int *save_stdout, int *save_stdint)
{
	*save_stdout = dup(1);
	*save_stdint = dup(0);
}

static void	ft_restore_fds(int saved_stdout, int saved_stdin)
{
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
}

static int	ft_exec_built_with_redir(t_shell *shell, t_cmd *cmd)
{
	int	save_stdout;
	int	save_stdin;
	int	exit_status;

	ft_save_fds(&save_stdout, &save_stdin);
	if (ft_setup_redirections(cmd->redirs) == -1)
	{
		shell->exit_status = 1;
		ft_restore_fds(save_stdout, save_stdin);
		return (1);
	}
	exit_status = ft_exec_builtin(shell, cmd->args);
	ft_restore_fds(save_stdout, save_stdin);
	return (exit_status);
}

static void	ft_wait_for_child(pid_t pid, t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else
		shell->exit_status = 128 + WTERMSIG(status);
}

/* OLD CODE - COMMENTED OUT
static void	ft_exec_external_cmd(t_cmd *cmd, char **env)
{
	char	*path;

	if (ft_setup_redirections(cmd->redirs) == -1)
		exit(1);
	path = ft_find_executable(cmd->args[0], env);
	if (!path)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": commend not found\n", 2);
		exit(127);
	}
	execve(path, cmd->args, env);
	perror("execve");
	exit(126);
}
*/

// NEW IMPLEMENTATION - Fixed external command execution with proper error handling
static void	ft_exec_external_cmd(t_cmd *cmd, char **env)
{
	char	*path;
	struct stat	statbuf;

	if (ft_setup_redirections(cmd->redirs) == -1)
		exit(1);
	
	// Check if command starts with / or ./ (absolute/relative path)
	if (ft_strchr(cmd->args[0], '/'))
	{
		if (stat(cmd->args[0], &statbuf) == 0)
		{
			if (S_ISDIR(statbuf.st_mode))
			{
				ft_putstr_fd(cmd->args[0], 2);
				ft_putstr_fd(": Is a directory\n", 2);
				exit(126);
			}
			if (access(cmd->args[0], X_OK) != 0)
			{
				ft_putstr_fd(cmd->args[0], 2);
				ft_putstr_fd(": Permission denied\n", 2);
				exit(126);
			}
		}
		else
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
	execve(path, cmd->args, env);
	perror("execve");
	exit(126);
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
/* OLD CODE - COMMENTED OUT
void	ft_exec_pipe(t_shell *shell, t_cmd *cmds)
{
	int		pipfd[2];
	int		prev_fd;
	t_cmd	*current;

	prev_fd = -1;
	current = cmds;
	while (current)
	{
		ft_creat_pip_poces(shell, current, prev_fd, pipfd);
		ft_clean_pip_fds(prev_fd, pipfd, current);
		if (current->next)
			prev_fd = pipfd[0];
		current = current->next;
	}
	while (wait(NULL) > 0)
		;
}
*/

// NEW IMPLEMENTATION - Fixed pipe execution with proper exit code handling
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
	// Wait for all children, but only use exit status from last command
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

/* OLD CODE - COMMENTED OUT
void	ft_exec_cmds(t_shell *shell, t_cmd *cmds)
{
	if (!cmds || !cmds->args)
		return ;
	if (!cmds->next)
		ft_exec_sing_cmd(shell, cmds);
	else
		ft_exec_pipe(shell, cmds);
}
*/

// NEW IMPLEMENTATION - Process heredocs before execution
void	ft_exec_cmds(t_shell *shell, t_cmd *cmds)
{
	if (!cmds || !cmds->args)
		return ;
	
	// Process all heredocs BEFORE starting any command execution
	ft_preprocess_heredocs(cmds);
	
	if (!cmds->next)
		ft_exec_sing_cmd(shell, cmds);
	else
		ft_exec_pipe(shell, cmds);
}
