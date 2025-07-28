
#include "../../include/minishell.h"

static int	ft_cd_handle_path(char **args, char **path, t_shell *shell)
{
	if (args[1] && args[2])
	{
		ft_print_command_error("cd", TOO_MANY_ARGS);
		return (1);
	}
	if (!args[1])
		return (ft_cd_handle_home(path, shell));
	else if (ft_strcmp(args[1], "-") == 0)
		return (ft_cd_handle_oldpwd(path, shell));
	else
		*path = args[1];
	return (0);
}

static int	ft_cd_change_dir(char *path)
{
	if (chdir(path) == -1)
	{
		if (errno == ENOENT)
			ft_print_command_error("cd", NO_SUCH_FILE);
		else if (errno == ENOTDIR)
			ft_print_command_error("cd", NOT_A_DIRECTORY);
		else
			ft_print_file_error(path);
		return (1);
	}
	return (0);
}

static int	ft_cd_update_env(t_shell *shell, char *old_pwd, int path_result)
{
	char	cwd[MAX_PATH];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		if (*old_pwd)
			ft_set_env_value(shell, "OLDPWD", old_pwd);
		ft_set_env_value(shell, "PWD", cwd);
		if (path_result == 2)
			ft_putendl_fd(cwd, 1);
	}
	return (0);
}

int	ft_builtin_cd(t_shell *shell, char **args)
{
	char	*path;
	char	*allocated_path;
	char	old_pwd[MAX_PATH];
	int		path_result;
	int		result;

	allocated_path = NULL;
	if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
		old_pwd[0] = '\0';
	path_result = ft_cd_handle_path(args, &path, shell);
	if (path_result == 1)
		return (1);
	if (!args[1] || ft_strcmp(args[1], "-") == 0)
		allocated_path = path;
	result = ft_cd_change_dir(path);
	if (allocated_path)
		free(allocated_path);
	if (result != 0)
		return (1);
	return (ft_cd_update_env(shell, old_pwd, path_result));
}
