
#include "../minishell.h"

char	*join_path(char *dir, char *cmd)
{
	char	*path;
	int		dir_len;
	int		cmd_len;

	if (!dir || !cmd)
		return (NULL);
	dir_len = strlen(dir);
	cmd_len = strlen(cmd);
	path = malloc(dir_len + cmd_len + 2);
	if (!path)
		return (NULL);
	strcpy(path, dir);
	if (dir[dir_len - 1] != '/')
		strcat(path, "/");
	strcat(path, cmd);
	return (path);
}

int	is_directory(char *path)
{
	struct stat	s;

	if (!path)
		return (0);
	if (stat(path, &s) == 0)
	{
		if (S_ISDIR(s.st_mode))
			return (1);
	}
	return (0);
}

void	print_error(char *cmd, char *msg)
{
	if (!cmd && !msg)
		return ;
	write(STDERR_FILENO, "minishell: ", 11);
	if (cmd)
	{
		write(STDERR_FILENO, cmd, strlen(cmd));
		write(STDERR_FILENO, ": ", 2);
	}
	if (msg)
		write(STDERR_FILENO, msg, strlen(msg));
	write(STDERR_FILENO, "\n", 1);
}

char	**split_args(char *str)
{
	char	**args;
	int		i;
	int		j;
	int		start;

	if (!str)
		return (NULL);
	args = malloc(sizeof(char *) * (MAX_ARGS + 1));
	if (!args)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i] && j < MAX_ARGS)
	{
		while (str[i] && (str[i] == ' ' || str[i] == '\t'))
			i++;
		if (!str[i])
			break ;
		start = i;
		while (str[i] && str[i] != ' ' && str[i] != '\t')
			i++;
		args[j++] = strndup(str + start, i - start);
	}
	args[j] = NULL;
	return (args);
}

char	*check_path_env(char *cmd, char *path_env)
{
	char	*path_copy;
	char	*dir;
	char	*full_path;
	char	*result;

	if (!path_env)
		return (NULL);
	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);
	dir = strtok(path_copy, ":");
	while (dir)
	{
		full_path = join_path(dir, cmd);
		if (!full_path)
			break ;
		if (access(full_path, X_OK) == 0 && !is_directory(full_path))
		{
			result = strdup(full_path);
			free(full_path);
			free(path_copy);
			return (result);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

char	*find_command_path(char *cmd, t_env *env_list)
{
	char	*path_env;

	if (!cmd || !cmd[0])
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0 && !is_directory(cmd))
			return (strdup(cmd));
		return (NULL);
	}
	path_env = get_env_value(env_list, "PATH");
	return (check_path_env(cmd, path_env));
}
