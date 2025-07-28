
#include "../../include/minishell.h"

void	ft_free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

void	ft_free_redirs(t_redir *redirs)
{
	t_redir	*tmp_redir;

	while (redirs)
	{
		tmp_redir = redirs;
		redirs = redirs->next;
		if (tmp_redir->file
			&& ft_strncmp(tmp_redir->file, "/tmp/minishell_heredoc_", 23) == 0)
			ft_cleanup_heredoc_file(tmp_redir->file);
		free(tmp_redir->file);
		free(tmp_redir);
	}
}

void	ft_free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp_cmd;
	int		i;

	while (cmds)
	{
		tmp_cmd = cmds;
		cmds = cmds->next;
		if (tmp_cmd->args)
		{
			i = 0;
			while (tmp_cmd->args[i])
				free(tmp_cmd->args[i++]);
			free(tmp_cmd->args);
		}
		ft_free_redirs(tmp_cmd->redirs);
		free(tmp_cmd);
	}
}

void	ft_cleanup_shell(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->env)
	{
		while (shell->env[i])
		{
			free(shell->env[i]);
			i++;
		}
		free(shell->env);
	}
	if (shell->cmds)
		ft_free_cmds(shell->cmds);
}

void	ft_cleanup_heredoc_file(char *filename)
{
	if (filename && access(filename, F_OK) == 0)
	{
		unlink(filename);
	}
}
