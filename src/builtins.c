#include "../minishell.h"

int	builtin_echo(t_command *cmd)
{
	int	i;
	int	n_flag;

	if (!cmd || !cmd->args)
		return (ERROR);
	i = 1;
	n_flag = 0;
	if (cmd->args[i] && strcmp(cmd->args[i], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
	return (SUCCESS);
}

int	builtin_cd(t_shell *shell, t_command *cmd)
{
	char	*path;
	char	cwd[MAX_PATH_LEN];

	if (!cmd || !cmd->args)
		return (ERROR);
	if (!cmd->args[1])
		path = get_env_value(shell->env_list, "HOME");
	else
		path = cmd->args[1];
	if (!path)
	{
		fprintf(stderr, "cd: HOME not set\n");
		return (ERROR);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return (ERROR);
	}
	if (getcwd(cwd, MAX_PATH_LEN))
		update_env(shell->env_list, "PWD", cwd);
	return (SUCCESS);
}

int	builtin_pwd(void)
{
	char	cwd[MAX_PATH_LEN];

	if (getcwd(cwd, MAX_PATH_LEN) == NULL)
	{
		perror("pwd");
		return (ERROR);
	}
	printf("%s\n", cwd);
	return (SUCCESS);
}

int	builtin_env(t_shell *shell)
{
	t_env	*env;

	if (!shell || !shell->env_list)
		return (ERROR);
	env = shell->env_list;
	while (env)
	{
		if (env->key && env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (SUCCESS);
}

int	builtin_exit(t_shell *shell, t_command *cmd)
{
	int	exit_status;

	printf("exit\n");
	if (!shell)
		exit(1);
	if (!cmd || !cmd->args || !cmd->args[1])
	{
		exit_status = shell->exit_status;
		free_shell(shell);
		exit(exit_status);
	}
	exit_status = atoi(cmd->args[1]);
	if (cmd->args[2])
	{
		fprintf(stderr, "exit: too many arguments\n");
		return (1);
	}
	free_shell(shell);
	exit(exit_status);
}
