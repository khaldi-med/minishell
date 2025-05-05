/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammed                                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-04                              #+#    #+#             */
/*   Updated: 2025-05-05                             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Create a new command structure
 */
t_command	*create_command(void)
{
	t_command	*new_cmd;
	int			i;

	new_cmd = malloc(sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	new_cmd->args = malloc(sizeof(char *) * MAX_ARGS);
	if (!new_cmd->args)
	{
		free(new_cmd);
		return (NULL);
	}
	i = 0;
	while (i < MAX_ARGS)
		new_cmd->args[i++] = NULL;
	new_cmd->in_fd = STDIN_FILENO;
	new_cmd->out_fd = STDOUT_FILENO;
	new_cmd->in_file = NULL;
	new_cmd->out_file = NULL;
	new_cmd->append = 0;
	new_cmd->heredoc = 0;
	new_cmd->heredoc_delim = NULL;
	new_cmd->next = NULL;
	return (new_cmd);
}

/**
 * Handle redirection tokens
 */
int	handle_redirections(t_command *cmd, t_token **curr)
{
	t_token	*token;
	t_token	*next;

	token = *curr;
	next = token->next;
	if (!next || next->type != TOKEN_WORD)
		return (ERROR);
	if (token->type == TOKEN_REDIRECT_IN)
		cmd->in_file = strdup(next->value);
	else if (token->type == TOKEN_REDIRECT_OUT)
	{
		cmd->out_file = strdup(next->value);
		cmd->append = 0;
	}
	else if (token->type == TOKEN_APPEND_OUT)
	{
		cmd->out_file = strdup(next->value);
		cmd->append = 1;
	}
	else if (token->type == TOKEN_HEREDOC)
		cmd->heredoc_delim = strdup(next->value);
	*curr = next;
	return (SUCCESS);
}

/**
 * Handle command arguments
 */
int	handle_arguments(t_command *cmd, t_token *token)
{
	int	i;

	i = 0;
	while (cmd->args[i] != NULL)
		i++;
	if (i >= MAX_ARGS - 1)
		return (ERROR);
	cmd->args[i] = strdup(token->value);
	if (!cmd->args[i])
		return (ERROR);
	cmd->args[i + 1] = NULL;
	return (SUCCESS);
}

/**
 * Expand environment variables in tokens
 */
int	expand_variables(t_shell *shell)
{
	t_token	*token;
	char	*expanded;

	if (!shell || !shell->tokens)
		return (ERROR);
	
	token = shell->tokens;
	while (token)
	{
		if (token->type == TOKEN_ENV_VARIABLE)
		{
			expanded = get_env_value(shell->env_list, token->value + 1);
			if (expanded)
			{
				free(token->value);
				token->value = strdup(expanded);
				if (!token->value)
					return (ERROR);
			}
		}
		token = token->next;
	}
	return (SUCCESS);
}

/**
 * Process a single token and update command structure
 */
void	process_token(t_command **curr, t_token **token)
{
	if ((*token)->type == TOKEN_PIPE && (*token)->next)
	{
		(*curr)->next = create_command();
		*curr = (*curr)->next;
	}
	else if ((*token)->type >= TOKEN_REDIRECT_IN && 
			(*token)->type <= TOKEN_HEREDOC)
		handle_redirections(*curr, token);
	else if ((*token)->type == TOKEN_WORD || (*token)->type == TOKEN_QUOTE)
		handle_arguments(*curr, *token);
}

/**
 * Parse tokens into command structures
 */
t_command	*parse_tokens(t_token *tokens)
{
	t_command	*cmds;
	t_command	*curr;
	t_token		*token;

	if (!tokens)
		return (NULL);
	cmds = create_command();
	if (!cmds)
		return (NULL);
	curr = cmds;
	token = tokens;
	while (token)
	{
		process_token(&curr, &token);
		token = token->next;
	}
	return (cmds);
}
