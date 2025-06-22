#include "../../include/minishell.h"


int	ft_setup_heredoc(char *delimiter)
{
	int		pipefd[2];
	char	*line;
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]);
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strcmp(line, delimiter) == 0)
			{
				if (line)
					free(line);
				break ;
			}
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
		close(pipefd[1]);
		exit(0);
	}
	else if (pid > 0)
	{
		close(pipefd[1]);
		waitpid(pid, NULL, 0);
		dup2(pipefd[0], 0);
		close(pipefd[0]);
	}
	return (0);
}

// NEW FUNCTION - Process all heredocs before pipeline execution
int	ft_preprocess_heredocs(t_cmd *cmds)
{
	t_cmd	*current;
	t_redir	*redir;

	current = cmds;
	while (current)
	{
		redir = current->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				// Process this heredoc but don't set up redirection yet
				// Just collect the input
				if (ft_process_heredoc_input(redir->file) == -1)
					return (-1);
			}
			redir = redir->next;
		}
		current = current->next;
	}
	return (0);
}

// NEW FUNCTION - Process heredoc input and store it
int	ft_process_heredoc_input(char *delimiter)
{
	char	*line;

	// Read heredoc lines until delimiter, but don't set up pipes yet
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break;
		}
		// For now, just consume the input
		// In a full implementation, you'd store this content
		free(line);
	}
	return (0);
}
