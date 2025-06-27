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
	return (1);
}
static int	ft_handle_export_arg(t_shell *shell, char *arg)
{
	char	*key;
	char	*value;
	char	*equal_pos;

	if (!ft_is_valid_identifier(arg))
	{
		ft_handle_export_error(arg);
		return (1);
	}
	equal_pos = ft_strchr(arg, '=');
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
		if (!ft_get_env_value(shell->env, key))
			ft_set_env_value(shell, key, "");
	}
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
