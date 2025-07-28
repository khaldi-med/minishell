
#include "../../include/minishell.h"

static char	*ft_process_heredoc_line(char *content, char *line)
{
	char	*temp;
	char	*new_content;
	char	*line_with_newline;

	temp = content;
	line_with_newline = ft_strjoin(line, "\n");
	if (!line_with_newline)
		return (NULL);
	if (ft_strlen(content) > 0)
		new_content = ft_strjoin(content, line_with_newline);
	else
		new_content = ft_strdup(line_with_newline);
	free(temp);
	free(line_with_newline);
	return (new_content);
}

int	ft_process_heredoc_redir(t_redir *redir)
{
	char	*temp_filename;

	temp_filename = ft_create_heredoc_file(redir->file);
	if (!temp_filename)
		return (-1);
	free(redir->file);
	redir->file = temp_filename;
	redir->type = TOKEN_REDIR_IN;
	return (0);
}

static int	ft_check_delimiter(char *line, char *delimiter)
{
	if (!line)
	{
		printf("\nminishell: warning: here-document delimited by end-of-file "
			"(wanted `%s')\n", delimiter);
		return (1);
	}
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

char	*ft_read_heredoc_input(char *delimiter)
{
	char	*line;
	char	*content;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	while (1)
	{
		line = readline("> ");
		if (ft_check_delimiter(line, delimiter))
			break ;
		if (!line)
			break ;
		content = ft_process_heredoc_line(content, line);
		free(line);
		if (!content)
			return (NULL);
	}
	return (content);
}

char	*ft_create_temp_filename(void)
{
	static int	count;
	char		*count_str;
	char		*filename;

	count = 0;
	count_str = ft_itoa(count++);
	if (!count_str)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", count_str);
	free(count_str);
	return (filename);
}
