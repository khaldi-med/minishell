
#include "../minishell.h"

/**
 * Determine token type based on the first character
 */
t_token_type	get_token_type(char c)
{
	if (c == '|')
		return (TOKEN_PIPE);
	else if (c == '<')
		return (TOKEN_REDIRECT_IN);
	else if (c == '>')
		return (TOKEN_REDIRECT_OUT);
	else if (c == ';')
		return (TOKEN_SEMICOLON);
	else if (c == ' ' || c == '\t')
		return (TOKEN_SPACE);
	else if (c == '\'' || c == '\"')
		return (TOKEN_QUOTE);
	else if (c == '$')
		return (TOKEN_ENV_VARIABLE);
	return (TOKEN_WORD);
}

/**
 * Create a new token with the given value and type
 */
t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = strdup(value);
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

/**
 * Add a new token to the end of the token list
 */
void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!new_token)
		return ;
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

/**
 * Copy quoted string content into result buffer
 */
void	copy_quoted_content(char *result, char *str, int start, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		result[i] = str[start + i];
		i++;
	}
	result[len] = '\0';
}

/**
 * Extract quoted text from input and advance position
 */
char	*handle_quotes(char *str, int *i)
{
	char	quote_type;
	char	*result;
	int		start;
	int		len;

	quote_type = str[*i];
	start = *i + 1;
	*i = *i + 1;
	while (str[*i] && str[*i] != quote_type)
		*i = *i + 1;
	if (!str[*i])
		return (NULL);
	len = *i - start;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	copy_quoted_content(result, str, start, len);
	*i = *i + 1;
	return (result);
}

/**
 * Handle regular (non-quoted) word tokens
 */
void	handle_word(char *input, int *i, t_token **tokens)
{
	char	buffer[1024];
	int		j;

	j = 0;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&& input[*i] != '\'' && input[*i] != '\"')
	{
		buffer[j++] = input[*i];
		(*i)++;
	}
	buffer[j] = '\0';
	add_token(tokens, create_token(buffer, get_token_type(buffer[0])));
}

/**
 * Handle quoted text tokens
 */
void	handle_quoted_text(char *input, int *i, t_token **tokens)
{
	char	*quoted;

	quoted = handle_quotes(input, i);
	if (quoted)
	{
		add_token(tokens, create_token(quoted, TOKEN_QUOTE));
		free(quoted);
	}
}

/**
 * Tokenize the input string into a list of tokens
 */
t_token	*tokenize(char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
			handle_quoted_text(input, &i, &tokens);
		else if (input[i] == ' ' || input[i] == '\t')
			i++;
		else
			handle_word(input, &i, &tokens);
	}
	return (tokens);
}
