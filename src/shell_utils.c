
#include "../minishell.h"

/**
 * Initialize the shell structure
 */
t_shell	*init_shell(char **env)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	/* Initialize all members to NULL/0 */
	shell->input = NULL;
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->env_list = init_env(env);
	shell->env_array = NULL;
	shell->exit_status = 0;
	shell->running = 1;
	if (!shell->env_list)
	{
		free(shell);
		return (NULL);
	}
	return (shell);
}

/**
 * Free the tokens linked list
 */
void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}

/**
 * Free arguments array in a command
 */
void	free_command_args(char **args)
{
	int	i;

	if (!args)
		return ;
	
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

/**
 * Free redirection files in a command
 */
void	free_command_files(t_command *cmd)
{
	if (!cmd)
		return ;
	
	if (cmd->in_file)
		free(cmd->in_file);
	if (cmd->out_file)
		free(cmd->out_file);
	if (cmd->heredoc_delim)
		free(cmd->heredoc_delim);
}

/**
 * Free the commands linked list
 */
void	free_commands(t_command *cmds)
{
	t_command	*temp;

	while (cmds)
	{
		temp = cmds;
		cmds = cmds->next;
		
		/* Free args array */
		if (temp->args)
			free_command_args(temp->args);
		
		/* Free redirection files */
		free_command_files(temp);
		
		free(temp);
	}
}

/**
 * Free the environment array
 */
void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

/**
 * Free all resources in the shell structure
 */
void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	
	if (shell->input)
		free(shell->input);
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->commands)
		free_commands(shell->commands);
	if (shell->env_list)
		free_env_list(shell->env_list);
	if (shell->env_array)
		free_env_array(shell->env_array);
	
	free(shell);
}

/**
 * Add a command to history
 */
void	add_to_history(char *input)
{
	add_history(input);
}

