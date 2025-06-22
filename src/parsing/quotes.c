#include "../../include/minishell.h"


char	*ft_handle_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	char	quote_char;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	quote_char = 0;
	while (str[i])
	{
		if (!quote_char && (str[i] == '\'' || str[i] == '"'))
		{
			quote_char = str[i];
			i++;
			continue ;
		}
		if (quote_char && str[i] == quote_char)
		{
			quote_char = 0;
			i++;
			continue ;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}
