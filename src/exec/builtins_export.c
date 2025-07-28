
#include "../../include/minishell.h"

int	ft_is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	if (ft_strchr(str, ';') || ft_strchr(str, '!')
		|| (ft_strchr(str, '=') && ft_strchr(ft_strchr(str, '=') + 1, '=')))
		return (0);
	return (1);
}

static int	ft_validate_quoted_arg(char *arg)
{
	char	*unquoted;

	if ((arg[0] == '"' && arg[ft_strlen(arg) - 1] == '"')
		|| (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\''))
	{
		unquoted = ft_substr(arg, 1, ft_strlen(arg) - 2);
		if (!ft_is_valid_identifier(unquoted))
		{
			ft_handle_export_error(arg);
			free(unquoted);
			return (0);
		}
		free(unquoted);
	}
	return (1);
}

static void	ft_handle_assignment(t_shell *shell, char *arg, char *equal_pos)
{
	char	*key;
	char	*value;
	char	*existing_value;

	if (equal_pos)
	{
		*equal_pos = '\0';
		key = arg;
		value = equal_pos + 1;
		ft_set_env_value(shell, key, value);
		*equal_pos = '=';
	}
	else
	{
		key = arg;
		existing_value = ft_get_env_value(shell->env, key);
		if (!existing_value)
			ft_set_env_value(shell, key, "");
		else
			free(existing_value);
	}
}

static int	ft_handle_export_arg(t_shell *shell, char *arg)
{
	char	*equal_pos;

	if (!arg || !*arg)
	{
		ft_handle_export_error(arg);
		return (1);
	}
	if (!ft_validate_quoted_arg(arg))
		return (1);
	if (!ft_is_valid_identifier(arg))
	{
		ft_handle_export_error(arg);
		return (1);
	}
	equal_pos = ft_strchr(arg, '=');
	ft_handle_assignment(shell, arg, equal_pos);
	return (0);
}

int	ft_builtin_export(t_shell *shell, char **args)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		ft_builtin_env(shell);
		return (0);
	}
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (ft_handle_export_arg(shell, args[i]))
			ret = 1;
		i++;
	}
	return (ret);
}
