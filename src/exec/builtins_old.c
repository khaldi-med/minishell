#include "../../include/minishell.h"

// Checks if the command is a builtin command
int	ft_is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

// Executes a builtin command
int	ft_exec_builtin(t_shell *shell, char **args)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (ft_builtin_echo(args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_builtin_cd(args));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_builtin_pwd());
	if (ft_strcmp(args[0], "export") == 0)
		return (ft_builtin_export(shell, args));
	if (ft_strcmp(args[0], "unset") == 0)
		return (ft_builtin_unset(shell, args));
	if (ft_strcmp(args[0], "env") == 0)
		return (ft_builtin_env(shell));
	if (ft_strcmp(args[0], "exit") == 0)
		return (ft_builtin_exit(shell, args));
	return (1);
}
{
	int	i;
	int	newline;
	int	first_arg;

	i = 1;
	first_arg = 1;
	newline = 1;
	while (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		if (!first_arg)
			ft_putstr_fd(" ", 1);
		ft_putstr_fd(args[i], 1);
		first_arg = 0;
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", 1);
	return (0);
}

int	ft_builtin_cd(char **args)
{
	char	*path;
	char	*home;

	if (args[1] && args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	if (!args[1])
	{
		home = getenv("HOME");
		if (!home)
		{
			ft_putstr_fd("cd: HOME not set\n", 1);
			return (1);
		}
		path = home;
	}
	else
		path = args[1];
	/* OLD CODE - COMMENTED OUT
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": NO such file or directory\n", 2);
		return (1);
	}
	*/
	
	// NEW IMPLEMENTATION - Fixed error message formatting
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	return (0);
}

int	ft_builtin_pwd(void)
{
	char	cwd[MAX_PATH];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd");
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	return (0);
}

int	ft_builtin_unset(t_shell *shell, char **args)
{
	int	i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		ft_unset_env_value(shell, args[i]);
		i++;
	}
	return (0);
}

int	ft_builtin_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		ft_putendl_fd(shell->env[i], 1);
		i++;
	}
	return (0);
}

static int	ft_is_valid_identifier(char *str)
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

int	ft_builtin_export(t_shell *shell, char **args)
{
	int		i;
	char	*key;
	char	*value;
	char	*equal_pos;
	int		ret;

	if (!args[1])
	{
		ft_builtin_env(shell);
		return (0);
	}
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (!ft_is_valid_identifier(args[i]))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret = 1;
		}
		else
		{
			equal_pos = ft_strchr(args[i], '=');
			if (equal_pos)
			{
				*equal_pos = '\0';
				key = args[i];
				value = equal_pos + 1;
				ft_set_env_value(shell, key, value);
				*equal_pos = '=';
			}
			else
			{
				key = args[i];
				if (!ft_get_env_value(shell->env, key))
					ft_set_env_value(shell, key, "");
			}
		}
		i++;
	}
	return (ret);
}

static int	ft_is_valid_numb(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[0] == '+' || str[0] == '-')
		i = 1;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/* OLD CODE - COMMENTED OUT
int	ft_builtin_exit(t_shell *shell, char **args)
{
	int	exit_code;

	if (args[1] && args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	if (args[1])
	{
		if (!ft_is_valid_numb(args[1]))
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			ft_cleanup_shell(shell);
			rl_clear_history();
			exit(2);
		}
		exit_code = ft_atoi(args[1]);
	}
	else
		exit_code = shell->exit_status;
	ft_cleanup_shell(shell);
	rl_clear_history();
	exit(exit_code & 255);
}
*/

// NEW IMPLEMENTATION - Fixed exit command logic
int	ft_builtin_exit(t_shell *shell, char **args)
{
	long	exit_code;

	ft_putstr_fd("exit\n", 2);  // Always print "exit" first
	if (args[1])
	{
		if (!ft_is_valid_numb(args[1]))
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			ft_cleanup_shell(shell);
			rl_clear_history();
			exit(2);
		}
		if (args[2])  // Check for too many arguments AFTER validating first arg
		{
			ft_putstr_fd("exit: too many arguments\n", 2);
			return (1);  // Don't exit, just return error
		}
		exit_code = ft_atoi(args[1]);
	}
	else
		exit_code = shell->exit_status;
	ft_cleanup_shell(shell);
	rl_clear_history();
	exit((unsigned char)exit_code);  // Proper modulo 256 handling
}
