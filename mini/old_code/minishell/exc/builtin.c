#include "../inc/minishell.h"

/* Get HOME directory from environment */
char *get_home_dir(t_bash *bash)
{
    if (!bash)
        return NULL;
    return get_env_value(bash->env_list, "HOME");
}

/*
 * for echo commend
 * */

int	builtin_echo(t_cmd *cmd)
{
	int	i;
	int	n_flag;
	char	*arg;

	if (!cmd || !cmd->arguments)
	{
		printf("\n");
		fflush(stdout);  // Ensure output is flushed
		return (0);
	}
	
	// Start from index 1 to skip the command name "echo"
	i = 1;
	n_flag = 0;
	
	// Handle multiple -n flags
	while (cmd->arguments[i])
	{
		arg = cmd->arguments[i];
		
		// Check if this argument is exactly "-n"
		if (ft_strcmp(arg, "-n") == 0)
		{
			n_flag = 1;
			i++;
		}
		// Check if this argument starts with "-n" and contains only 'n' characters
		else if (arg[0] == '-' && arg[1] == 'n')
		{
			int j = 1;
			int valid_n_flag = 1;
			
			// Check if all characters after '-' are 'n'
			while (arg[j])
			{
				if (arg[j] != 'n')
				{
					valid_n_flag = 0;
					break;
				}
				j++;
			}
			
			if (valid_n_flag)
			{
				n_flag = 1;
				i++;
			}
			else
				break; // Not a valid -n flag, treat as regular argument
		}
		else
			break; // Not a -n flag, treat as regular argument
	}
	
	// Print the remaining arguments
	while (cmd->arguments[i])
	{
		printf("%s", cmd->arguments[i]);
		if (cmd->arguments[i + 1])
			printf(" ");
		i++;
	}
	
	// Print newline unless -n flag was used
	if (!n_flag)
		printf("\n");
	
	// Always flush output to ensure proper behavior
	fflush(stdout);
	
	return (0);
}

/**
 *for pwd commend
 */

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

/*
 * cd commend
 * */

int builtin_cd(t_bash *bash, t_cmd *cmd)
{
    char *path;
    char old_cwd[MAX_PATH_LEN];
    char new_cwd[MAX_PATH_LEN];

    if (!bash || !cmd || !cmd->arguments)
        return (ERROR);

    // Check for too many arguments
    if (cmd->arguments[1] && cmd->arguments[2])
    {
        fprintf(stderr, "cd: too many arguments\n");
        return (ERROR);
    }

    if (!cmd->arguments[1])
        path = get_home_dir(bash);
    else
        path = cmd->arguments[1];

    if (!path)
    {
        fprintf(stderr, "cd: HOME not set\n");
        return (ERROR);
    }

    if (getcwd(old_cwd, MAX_PATH_LEN) == NULL)
    {
        perror("cd");
        return (ERROR);
    }

    if (chdir(path) != 0)
    {
        perror("cd");
        return (ERROR);
    }

    if (getcwd(new_cwd, MAX_PATH_LEN) == NULL)
    {
        perror("cd");
        return (ERROR);
    }

    if (update_env(bash, "OLDPWD", old_cwd) == ERROR ||
        update_env(bash, "PWD", new_cwd) == ERROR)
    {
        fprintf(stderr, "cd: failed to update directory variables\n");
        return (ERROR);
    }

    return (SUCCESS);
}
