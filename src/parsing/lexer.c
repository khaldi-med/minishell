#include "../../include/minishell.h"

static t_token	*ft_handle_pipe(int *i)
{
	(*i)++;
	return (ft_creat_token("|", TOKEN_PIPE));
}

static char	*ft_extract_word(char *input, int *i)
{
	int		start;
	int		len;
	char	*word;
	char	quote;

	start = *i;
	quote = 0;
	while (input[*i])
	{
		if (!quote && (input[*i] == '\'' || input[*i] == '"'))
			quote = input[*i];
		else if (quote && input[*i] == quote)
			quote = 0;
		else if (!quote &&
					(input[*i] == ' ' || input[*i] == '\t' || input[*i] == '<'
							||
					input[*i] == '>' || input[*i] == '|'))
			break ;
		(*i)++;
	}
	len = *i - start;
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	ft_strlcpy(word, input + start, len + 1);
	word[len] = '\0';
	return (word);
}

static t_token	*ft_handle_word(char *input, int *i)
{
	char	*word;
	t_token	*token;

	word = ft_extract_word(input, i);
	token = ft_creat_token(word, TOKEN_WORD);
	free(word);
	return (token);
}

t_token	*ft_tokenize(char *input)
{
	t_token	*tokens;
	t_token	*new_token;
	int		i;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		i = ft_skip_space(input, i);
		if (!input[i])
			break ;
		if (input[i] == '|')
			new_token = ft_handle_pipe(&i);
		else if (input[i] == '<' || input[i] == '>')
			new_token = ft_handle_redirection(input, &i);
		else
			new_token = ft_handle_word(input, &i);
		ft_add_token(&tokens, new_token);
	}
	return (tokens);
}
