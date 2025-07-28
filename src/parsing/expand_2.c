
#include "../../include/minishell.h"

int	ft_in_singlea_q(char *s, int pos)
{
	int	i;
	int	in_single;
	int	in_double;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (i < pos)
	{
		if (s[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (s[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single);
}

char	*ft_get_variable_value(char *var, t_shell *shell)
{
	char	*value;
	char	*exit_stat_str;

	if (ft_strcmp(var, "?") == 0)
	{
		exit_stat_str = ft_itoa(shell->exit_status);
		return (exit_stat_str);
	}
	value = ft_get_env_value(shell->env, var);
	if (value)
		return (value);
	return (ft_strdup(""));
}

char	*ft_extract_quoted_var(char *str, int *i)
{
	int		start;
	int		len;
	char	*var_name;
	char	quote_char;

	quote_char = str[*i];
	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != quote_char)
		(*i)++;
	if (str[*i] == quote_char)
	{
		len = *i - start;
		(*i)++;
		var_name = malloc(len + 1);
		ft_strlcpy(var_name, str + start, len + 1);
		var_name[len] = '\0';
		return (var_name);
	}
	return (NULL);
}

char	*ft_handle_dollar_expansion(char *str, int *i, char *result,
		t_shell *shell)
{
	char	*expanded;
	char	*tmp;

	expanded = ft_expand_dollar(str, i, shell);
	tmp = ft_strjoin(result, expanded);
	free(result);
	free(expanded);
	return (tmp);
}
