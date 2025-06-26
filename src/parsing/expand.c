#include "../../include/minishell.h"

static int	ft_in_singlea_q(char *s, int pos)
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

static char	*ft_extract_var_name(char *str, int *i)
{
	int		start;
	int		len;
	char	*var_name;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	if (!(ft_isalpha(str[*i]) || str[*i] == '_'))
		return (NULL);
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	len = *i - start;
	var_name = malloc(len + 1);
	ft_strlcpy(var_name, str + start, len + 1);
	var_name[len] = '\0';
	return (var_name);
}

char	*ft_expand_dollar(char *s, int *i, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	int		start;

	start = *i;
	(*i)++;
	var_name = ft_extract_var_name(s, i);
	if (!var_name)
	{
		*i = start + 1;
		return (ft_strdup("$"));
	}
	var_value = ft_get_variable_value(var_name, shell);
	free(var_name);
	return (var_value);
}

char	*ft_expand_variables(char *str, t_shell *shell)
{
	char	*result;
	char	*expanded;
	char	*tmp;
	int		i;
	int		result_len;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && !ft_in_singlea_q(str, i))
		{
			expanded = ft_expand_dollar(str, &i, shell);
			tmp = ft_strjoin(result, expanded);
			free(result);
			free(expanded);
			result = tmp;
		}
		else
		{
			result_len = ft_strlen(result);
			tmp = malloc(result_len + 2);
			ft_strlcpy(tmp, result, result_len + 1);
			tmp[result_len] = str[i];
			tmp[result_len + 1] = '\0';
			free(result);
			result = tmp;
			i++;
		}
	}
	return (result);
}
