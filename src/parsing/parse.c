#include "../../include/minishell.h"

t_cmd	*ft_creat_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

static void	ft_process_word_token(t_cmd *cmd, t_token *token, int *index,
		t_shell *shell)
{
	char	*expanded_value;
	char	*processed_value;

	expanded_value = ft_expand_variables(token->value, shell);
	processed_value = ft_handle_quotes(expanded_value);
	cmd->args[*index] = processed_value;
	free(expanded_value);
	(*index)++;
}

static int	ft_count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens->type == TOKEN_WORD)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

void	ft_add_redir(t_cmd *cmd, t_token_type type, char *file)
{
	t_redir	*new_redir;
	t_redir	*current;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	new_redir->type = type;
	new_redir->file = ft_strdup(file);
	new_redir->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = new_redir;
	else
	{
		current = cmd->redirs;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
}

static void	ft_process_redir_token(t_cmd *cmd, t_token **token, t_shell *shell)
{
	t_token_type	redir_type;
	char			*expanded_value;
	char			*processed_value;

	redir_type = (*token)->type;
	*token = (*token)->next;
	if (*token && (*token)->type == TOKEN_WORD)
	{
		expanded_value = ft_expand_variables((*token)->value, shell);
		processed_value = ft_handle_quotes(expanded_value);
		ft_add_redir(cmd, redir_type, processed_value);
		free(expanded_value);
		free(processed_value);
	}
}

static void	ft_pars_args(t_cmd *cmd, t_token **tokens, t_shell *shell)
{
	int		arg_count;
	int		i;
	t_token	*current;

	arg_count = ft_count_args(*tokens);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return ;
	i = 0;
	current = *tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
			ft_process_word_token(cmd, current, &i, shell);
		else if (current->type >= TOKEN_REDIR_IN
				&& current->type <= TOKEN_HEREDOC)
			ft_process_redir_token(cmd, &current, shell);
		current = current->next;
	}
	cmd->args[i] = NULL;
	*tokens = current;
}

t_cmd	*ft_parse_tokens(t_token *tokens, t_shell *shell)
{
	t_cmd	*cmds;
	t_cmd	*current_cmd;
	t_cmd	*new_cmd;

	if (!tokens)
		return (NULL);
	cmds = ft_creat_cmd();
	current_cmd = cmds;
	while (tokens)
	{
		ft_pars_args(current_cmd, &tokens, shell);
		if (tokens && tokens->type == TOKEN_PIPE)
		{
			tokens = tokens->next;
			new_cmd = ft_creat_cmd();
			current_cmd->next = new_cmd;
			current_cmd = new_cmd;
		}
	}
	return (cmds);
}
