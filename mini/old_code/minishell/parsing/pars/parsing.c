#include "../../inc/minishell.h"

// Helper function to free redirection structures
static void	free_redirections(t_red **redirections)
{
	int	i;

	if (!redirections)
		return;
		
	i = 0;
	while (redirections[i])
	{
		if (redirections[i]->file)
			free(redirections[i]->file);
		free(redirections[i]);
		i++;
	}
	free(redirections);
}

// Helper function to free environment variables linked list
void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

// Helper function to free string array
static void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return;
		
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_cmd_array(t_bash *bash)
{
	int	i;

	i = 0;
	if (!bash || !bash->s_cmd)
		return;
		
	while (i < bash->num_cmd)
	{
		if (bash->s_cmd[i])
		{
			// Free command string
			if (bash->s_cmd[i]->command)
				free(bash->s_cmd[i]->command);
				
			// Free redirections array with proper cleanup
			if (bash->s_cmd[i]->s_red)
				free_redirections(bash->s_cmd[i]->s_red);
				
			// Free environment variables list
			if (bash->s_cmd[i]->s_env)
				free_env_list(bash->s_cmd[i]->s_env);
				
			// Free arguments array
			if (bash->s_cmd[i]->arguments)
				free_string_array(bash->s_cmd[i]->arguments);
				
			// Finally free the command structure
			free(bash->s_cmd[i]);
			bash->s_cmd[i] = NULL;
		}
		i++;
	}
	free(bash->s_cmd);
	bash->s_cmd = NULL;
}

int	select_struct1(t_bash *bash)
{
	char	*cmd;

	// Initialize bash fields
	bash->commands = NULL;
	bash->args_pip = NULL;
	bash->s_cmd = NULL;

	// Read command from user
	cmd = readline("minishell$ ");
	if (!cmd)
	{
		bash->num_cmd = 0;
		return (0);
	}
	
	// Check for syntax errors
	if (!check_cmd(cmd) || !check_cmd1(cmd))
	{
		bash->num_cmd = 0;
		free(cmd);
		return (0);
	}
	
	// Count pipes to determine number of commands
	bash->num_cmd = count_pipes(cmd);
	
	// Allocate memory for command structures
	bash->s_cmd = malloc(sizeof(t_cmd *) * (bash->num_cmd + 1));
	if (!bash->s_cmd)
	{
		free(cmd);
		return (0);
	}
	
	// Save the full command string
	bash->commands = ft_strdup(cmd);
	if (!bash->commands)
	{
		free(cmd);
		free(bash->s_cmd);
		bash->s_cmd = NULL;
		return (0);
	}
	
	// Split command by pipes
	bash->args_pip = ft_split(cmd, '|');
	if (!bash->args_pip)
	{
		free(cmd);
		free(bash->commands);
		bash->commands = NULL;
		free(bash->s_cmd);
		bash->s_cmd = NULL;
		return (0);
	}
	
	free(cmd);
	return (1);
}

int	select_struct2(t_bash *bash)
{
	int	i;

	i = 0;
	while (i < bash->num_cmd)
	{
		bash->s_cmd[i] = malloc(sizeof(t_cmd));
		if (!bash->s_cmd[i])
			return (0);
		bash->s_cmd[i]->command = ft_strdup(bash->args_pip[i]);
		bash->s_cmd[i]->s_red = NULL;
		bash->s_cmd[i]->s_env = NULL;
		bash->s_cmd[i]->check_env = 0;
		bash->s_cmd[i]->check_red = 0;
		i++;
	}
	bash->s_cmd[i] = NULL;
	if (!red_parse(bash))
	{
		printf("bash: syntax error near unexpected token `newline'\n");
		free_cmd_array(bash);
		return (0);
	}
	return (1);
}

int	select_struct3(t_bash *bash)
{
	int	i;

	i = 0;
	while (bash->s_cmd[i])
	{
		if (!bash->s_cmd[i]->command)
			return (0);
		
		// Tokenize the command
		bash->s_cmd[i]->arguments = tokenizer(bash->s_cmd[i]->command);
		if (!bash->s_cmd[i]->arguments)
			return (0);
		
		// Remove redirection tokens from arguments
		remove_redirection_from_args(bash->s_cmd[i]);
		
		// Expand environment variables in arguments
		expand_command_variables(bash, bash->s_cmd[i]);
		
		i++;
	}
	return (1);
}

void	select_struct(t_bash *bash)
{
	if (!select_struct1(bash))
		return ;
	if (!select_struct2(bash))
		return ;
	if (!select_struct3(bash))
		return ;
	check_red_env(bash);
}
/* Test main function removed to avoid duplicate main definitions */
