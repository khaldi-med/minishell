#include "../../include/minishell.h"

static int	ft_create_heredoc_file(char *delimiter)
{
	char	*line;
	char	*content;
	char	*temp;
	int		pipefd[2];

	if (!delimiter || !*delimiter)
		return (-1);

	// Create a pipe for heredoc content
	if (pipe(pipefd) == -1)
		return (-1);

	content = ft_strdup("");
	if (!content)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("\nminishell: warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		// Add line to content with newline
		temp = content;
		if (ft_strlen(content) > 0)
			content = ft_strjoin_with_newline(content, line);
		else
			content = ft_strjoin(line, "\n");
		free(temp);
		free(line);
		if (!content)
		{
			close(pipefd[0]);
			close(pipefd[1]);
			return (-1);
		}
	}

	// Write content to pipe and close write end
	if (ft_strlen(content) > 0)
		write(pipefd[1], content, ft_strlen(content));
	close(pipefd[1]);
	free(content);

	return (pipefd[0]); // Return read end for later use
}

int	ft_preprocess_heredocs(t_cmd *cmds)
{
	t_cmd	*current;
	t_redir	*redir;
	int		fd;
	char	*temp_filename;
	char	*fd_str;

	current = cmds;
	while (current)
	{
		redir = current->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				fd = ft_create_heredoc_file(redir->file);
				if (fd == -1)
					return (-1);
				// Convert fd to string and replace the delimiter with the fd
				fd_str = ft_itoa(fd);
				if (!fd_str)
				{
					close(fd);
					return (-1);
				}
				temp_filename = ft_strjoin("/dev/fd/", fd_str);
				free(fd_str);
				if (!temp_filename)
				{
					close(fd);
					return (-1);
				}
				free(redir->file);
				redir->file = temp_filename;
				redir->type = TOKEN_REDIR_IN; // Change to input redirection
			}
			redir = redir->next;
		}
		current = current->next;
	}
	return (0);
}

