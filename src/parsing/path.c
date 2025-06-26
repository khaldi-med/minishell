#include "../../include/minishell.h"

int	ft_is_executable(char *path)
{
	struct stat	file_stat;

	if (stat(path, &file_stat) == 0)
	{
		if (S_ISREG(file_stat.st_mode) && (file_stat.st_mode & S_IXUSR))
			return (1);
	}
	return (0);
}

static char	*ft_check_dir_path(char *cmd)
{
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (ft_is_executable(cmd))
			return (ft_strdup(cmd));
	}
	return (NULL);
}

static char	*ft_search_in_path_dirs(char *cmd, char **path_dirs)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		temp = ft_strjoin(path_dirs[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (ft_is_executable(full_path))
		{
			ft_free_path_dirs(path_dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*ft_find_executable(char *cmd, char **env)
{
	char	**path_dirs;
	char	*result;

	if (!cmd)
		return (NULL);
	result = ft_check_dir_path(cmd);
	if (result)
		return (result);
	path_dirs = ft_get_path_dirs(env);
	if (!path_dirs)
		return (NULL);
	result = ft_search_in_path_dirs(cmd, path_dirs);
	if (result)
		return (result);
	ft_free_path_dirs(path_dirs);
	return (NULL);
}
