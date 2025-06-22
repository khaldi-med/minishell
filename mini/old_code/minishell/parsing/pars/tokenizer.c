#include "../../inc/minishell.h"

void	free_double_pointer_tk(char **arr, int p)
{
	if (!arr)
		return ;
	while (p >= 0)
	{
		free(arr[p]);
		p--;
	}
	free(arr);
}

// New helper function to check if character is a quote
static int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

// New helper function to handle escaped characters
static int	is_escaped(char *str, int i)
{
	int count;
	
	count = 0;
	while (i > 0 && str[i - 1] == '\\')
	{
		count++;
		i--;
	}
	return (count % 2);
}

int	check_tokinzer(char c)
{
	if (!is_quote(c) && (c == ' ' || c == '\t'))
		return (1);
	return (0);
}

// New function to check for redirection operators
int	is_redirection_operator(char c)
{
	return (c == '>' || c == '<');
}

// New function to check for pipe
int	is_pipe(char c)
{
	return (c == '|');
}

// New function to skip quoted content
static int	skip_quotes(char *cmd, int i, char quote_type)
{
	i++; // Skip the opening quote
	while (cmd[i] && cmd[i] != quote_type)
		i++;
	return (cmd[i] ? i + 1 : i); // Return position after closing quote or end of string
}

int	count_word_tk(char *cmd)
{
	int	i;
	int	j;
	char quote_type;

	i = 0;
	j = 0;
	while (cmd[i])
	{
		while (check_tokinzer(cmd[i]) && cmd[i])
			i++;
		if (!cmd[i])
			break;
		if (is_quote(cmd[i]) && !is_escaped(cmd, i))
		{
			quote_type = cmd[i];
			i = skip_quotes(cmd, i, quote_type);
			if (!cmd[i - 1]) // Check if quote was closed
				return (-1); // Error: unclosed quote
			j++;
		}
		else if (!check_tokinzer(cmd[i]))
		{
			j++;
			while (cmd[i] && (!check_tokinzer(cmd[i]) || 
				   (is_quote(cmd[i]) && !is_escaped(cmd, i))))
			{
				if (is_quote(cmd[i]) && !is_escaped(cmd, i))
					i = skip_quotes(cmd, i, cmd[i]);
				else
					i++;
			}
		}
	}
	return (j);
}

// Helper function to copy quoted content WITH quotes preserved
static int	copy_quoted_content(char *dst, char *src, int start, int *len)
{
	int	i;
	char quote_type;

	quote_type = src[start];
	i = 0;
	
	// Include opening quote
	dst[*len] = src[start];
	(*len)++;
	start++;
	
	// Copy everything inside quotes, including special characters
	while (src[start + i] && src[start + i] != quote_type)
	{
		dst[*len] = src[start + i];
		(*len)++;
		i++;
	}
	
	// Include closing quote if it exists
	if (src[start + i] == quote_type)
	{
		dst[*len] = src[start + i];
		(*len)++;
		i++;
	}
	
	// Return total chars processed
	return (i + 1);
}


char	**alloc_words_tk(char **ret, char *cmd, int word)
{
	int	i;
	int	x;
	int	len;
	char	*tmp;

	i = 0;
	x = 0;
	while (x < word)
	{
		while (check_tokinzer(cmd[i]) && cmd[i])
			i++;
		len = 0;
		tmp = malloc(ft_strlen(cmd) + 1); // Allocate max possible length
		if (!tmp)
		{
			free_double_pointer_tk(ret, x - 1);
			return (NULL);
		}
		while (cmd[i])
		{
			if (is_quote(cmd[i]) && !is_escaped(cmd, i))
				i += copy_quoted_content(tmp, cmd, i, &len);
			else if (!check_tokinzer(cmd[i]))
			{
				tmp[len++] = cmd[i++];
			}
			else
				break;
		}
		tmp[len] = '\0';
		ret[x] = ft_strdup(tmp);
		free(tmp);
		if (!ret[x])
		{
			free_double_pointer_tk(ret, x - 1);
			return (NULL);
		}
		x++;
	}
	ret[x] = NULL;
	return (ret);
}

char	**tokenizer(char *cmd)
{
	int word;
	char **ret;

	if (!cmd)
		return (NULL);
	word = count_word_tk(cmd);
	if (word == -1)
	{
		fprintf(stderr, "minishell: syntax error: unclosed quote\n");
		return (NULL);
	}
	ret = malloc(sizeof(char *) * (word + 1));
	if (!ret)
		return (NULL);
	ret = alloc_words_tk(ret, cmd, word);
	if (!ret)
		return (NULL);
	return (ret);
}
