#include "../../include/minishell.h"

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
				if (ft_process_heredoc_input(redir->file) == -1)
					return (-1);
			}
			redir = redir->next;
		}
		current = current->next;
	}
	return (0);
}

int	ft_process_heredoc_input(char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		free(line);
	}
	return (0);
}
