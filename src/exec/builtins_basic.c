#include "../../include/minishell.h"

int	ft_builtin_echo(char **args)
{
	int	i;
	int	newline;
	int	first_arg;
	int	j;
	int	valid_n_flag;

	i = 1;
	first_arg = 1;
	newline = 1;
	
	/* Process -n flags - must be exactly "-n" followed by only 'n' characters */
	while (args[i])
	{
		if (args[i][0] != '-' || args[i][1] == '\0')
			break;
		
		valid_n_flag = 1;
		j = 1;
		while (args[i][j])
		{
			if (args[i][j] != 'n')
			{
				valid_n_flag = 0;
				break;
			}
			j++;
		}
		
		if (valid_n_flag && j > 1) /* At least one 'n' after '-' */
		{
			newline = 0;
			i++;
		}
		else
			break;
	}
	
	/* Print arguments */
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

int	ft_builtin_cd(t_shell *shell, char **args)
{
	char	*path;
	char	*home;
	char	cwd[MAX_PATH];
	char	old_pwd[MAX_PATH];

	if (args[1] && args[2])
	{
		ft_print_command_error("cd", TOO_MANY_ARGS);
		return (1);
	}
	
	/* Save current directory as old_pwd using getcwd (allowed function) */
	if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
	{
		old_pwd[0] = '\0';
	}
	
	if (!args[1])
	{
		home = getenv("HOME");  /* getenv is allowed */
		if (!home)
		{
			ft_print_command_error("cd", "HOME not set");
			return (1);
		}
		path = home;
	}
	else
		path = args[1];
		
	if (chdir(path) == -1)  /* chdir is allowed */
	{
		if (errno == ENOENT)
			ft_print_command_error("cd", NO_SUCH_FILE);
		else if (errno == ENOTDIR)
			ft_print_command_error("cd", NOT_A_DIRECTORY);
		else
			ft_print_file_error(path);
		return (1);
	}
	
	/* Update PWD and OLDPWD using your existing ft_set_env_value function */
	if (getcwd(cwd, sizeof(cwd)) != NULL)  /* getcwd is allowed */
	{
		/* Set OLDPWD to previous directory */
		if (*old_pwd)
			ft_set_env_value(shell, "OLDPWD", old_pwd);
		/* Set PWD to new directory */
		ft_set_env_value(shell, "PWD", cwd);
	}
	
	return (0);
}

int	ft_builtin_pwd(void)
{
	char	cwd[MAX_PATH];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		ft_print_file_error("pwd");
		return (1);
	}
	ft_putendl_fd(cwd, 1);
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
