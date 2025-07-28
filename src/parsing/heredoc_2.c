
#include "../../include/minishell.h"

static int	ft_process_cmd_heredocs(t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
		{
			if (ft_process_heredoc_redir(redir) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

int	ft_preprocess_heredocs(t_cmd *cmds)
{
	t_cmd	*current;

	current = cmds;
	while (current)
	{
		if (ft_process_cmd_heredocs(current) == -1)
			return (-1);
		current = current->next;
	}
	return (0);
}

static int	ft_write_content_to_fd(int fd, char *content, char *temp_filename)
{
	int	bytes_written;

	if (ft_strlen(content) > 0)
	{
		bytes_written = write(fd, content, ft_strlen(content));
		if (bytes_written == -1)
		{
			perror("write");
			close(fd);
			unlink(temp_filename);
			return (-1);
		}
	}
	return (0);
}

static int	ft_open_temp_file(char *temp_filename)
{
	return (open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600));
}

char	*ft_create_heredoc_file(char *delimiter)
{
	char	*content;
	char	*temp_filename;
	int		fd;

	if (!delimiter || !*delimiter)
		return (NULL);
	content = ft_read_heredoc_input(delimiter);
	if (!content)
		return (NULL);
	temp_filename = ft_create_temp_filename();
	if (!temp_filename)
	{
		free(content);
		return (NULL);
	}
	fd = ft_open_temp_file(temp_filename);
	if (fd == -1 || ft_write_content_to_fd(fd, content, temp_filename) == -1)
	{
		free(content);
		free(temp_filename);
		return (NULL);
	}
	close(fd);
	free(content);
	return (temp_filename);
}
