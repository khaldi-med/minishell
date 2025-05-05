#include "../minishell.h"

/**
 * Check if input is the exit command
 */
int	check_exit_command(t_shell *shell)
{
	if (strcmp(shell->input, "exit") == 0)
	{
		shell->running = 0;
		return (0);
	}
	return (1);
}

/**
 * Tokenize and expand input
 */
int	tokenize_and_expand(t_shell *shell)
{
	shell->tokens = tokenize(shell->input);
	if (!shell->tokens)
		return (ERROR);
	
	expand_variables(shell);
	return (SUCCESS);
}

/**
 * Parse tokens into commands
 */
int	parse_input(t_shell *shell)
{
	shell->commands = parse_tokens(shell->tokens);
	if (!shell->commands)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return (ERROR);
	}
	return (SUCCESS);
}

/**
 * Cleanup after command execution
 */
void	cleanup_after_execution(t_shell *shell)
{
	free_tokens(shell->tokens);
	free_commands(shell->commands);
	shell->tokens = NULL;
	shell->commands = NULL;
}

/**
 * Process user input
 * Returns 1 if the shell should continue running, 0 if it should exit
 */
int	process_input(t_shell *shell)
{
	int	status;

	/* Handle empty input */
	if (!shell->input || shell->input[0] == '\0')
		return (1);
	
	/* Check for exit command */
	if (!check_exit_command(shell))
		return (0);
	
	/* Tokenize and expand input */
	if (tokenize_and_expand(shell) == ERROR)
		return (1);
	
	/* Parse tokens into commands */
	if (parse_input(shell) == ERROR)
		return (1);
	
	/* Execute commands */
	status = execute_commands(shell);
	shell->exit_status = status;
	
	/* Cleanup */
	cleanup_after_execution(shell);
	
	return (1);
}

/**
 * Handle user input (readline, history)
 */
void	handle_user_input(t_shell *shell)
{
	/* Read input using readline */
	shell->input = readline(PROMPT);
	
	/* Handle EOF (Ctrl+D) */
	if (!shell->input)
	{
		printf("exit\n");
		shell->running = 0;
		return;
	}
	
	/* Add input to history if not empty */
	if (shell->input[0] != '\0')
		add_history(shell->input);
}

/**
 * Main function of the shell
 */
int	main(int argc, char **argv, char **env)
{
	t_shell	*shell;
	int		exit_status;

	(void)argc;
	(void)argv;
	
	/* Initialize the shell */
	shell = init_shell(env);
	if (!shell)
	{
		fprintf(stderr, "Failed to initialize shell\n");
		return (1);
	}
	
	/* Set up signal handlers */
	setup_signals();
	
	/* Main shell loop */
	while (shell->running)
	{
		handle_user_input(shell);
		if (shell->input)
		{
			process_input(shell);
			free(shell->input);
			shell->input = NULL;
		}
	}
	
	/* Save exit status before cleanup */
	exit_status = shell->exit_status;
	
	/* Clean up and exit */
	free_shell(shell);
	return (exit_status);
}
