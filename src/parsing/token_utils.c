#include "../../include/minishell.h"

int	ft_skip_space(char *input, int i)
{
	while (input[i] && (input[i] == ' ' || input[i] == '\t'))
		i++;
	return (i);
}

t_token	*ft_creat_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->next = NULL;
	return (token);
}

void	ft_add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

static t_token_type	ft_get_redir_type(char *input, int *i)
{
	if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			(*i)++;
			return (TOKEN_HEREDOC);
		}
		return (TOKEN_REDIR_IN);
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			(*i)++;
			return (TOKEN_REDIR_APPEND);
		}
		return (TOKEN_REDIR_OUT);
	}
	return (TOKEN_WORD);
}

t_token	*ft_handle_redirection(char *input, int *i)
{
	t_token_type	type;
	t_token			*token;

	type = ft_get_redir_type(input, i);
	if (type == TOKEN_HEREDOC)
		token = ft_creat_token("<<", type);
	else if (type == TOKEN_REDIR_APPEND)
		token = ft_creat_token(">>", type);
	else if (type == TOKEN_REDIR_IN)
		token = ft_creat_token("<", type);
	else
		token = ft_creat_token(">", type);
	(*i)++;
	return (token);
}
