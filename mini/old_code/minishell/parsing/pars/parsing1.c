#include "../../inc/minishell.h"

// Helper function to check consecutive pipes
static int	check_consecutive_pipes(const char *str, int i)
{
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	return (str[i] == '|');
}


// Helper function to skip whitespace
static int	skip_whitespace(const char *str, int i)
{
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	return (i);
}

// Check redirection syntax
static int	check_redirection_syntax(const char *str, int i)
{
	int		count;
	char	redir_type;

	redir_type = str[i];
	count = 1;
	i++;
	// Count consecutive redirection operators
	while (str[i] == redir_type)
	{
		count++;
		i++;
	}
	
	// Check for too many consecutive redirections
	if (count > 2)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `%c'\n", redir_type);
		return (0);
	}
	
	i = skip_whitespace(str, i);
	
	// Check for missing redirection target
	if (!str[i])
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
		return (0);
	}
	
	// Check if redirection is followed by another redirection or pipe
	if (str[i] == '<' || str[i] == '>')
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `%c'\n", str[i]);
		return (0);
	}
	
	if (str[i] == '|')
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
		return (0);
	}
	
	return (1);
}


// Check for empty pipe segments
static int	check_empty_pipe_segments(char *str)
{
	int	i;
	int	found_char;

	i = 0;
	found_char = 0;
	while (str[i])
	{
		if (str[i] == '|')
		{
			if (!found_char)
			{
				fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
				return (0);
			}
			found_char = 0;
		}
		else if (str[i] != ' ' && str[i] != '\t')
			found_char = 1;
		i++;
	}
	if (!found_char && i > 0)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
		return (0);
	}
	return (1);
}

// Main syntax checking function
int	check_cmd(char *str)
{
	int		i;
	char	quote;

	i = 0;
	if (!str || !*str)
		return (1);

	// Check if command starts with a pipe
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '|')
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
		return (0);
	}

	// Process the command string
	while (str[i])
	{
		// Handle quotes
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			if (!str[i])
			{
				fprintf(stderr, "minishell: syntax error: unclosed quote\n");
				return (0);
			}
		}
		// Handle pipes
		else if (str[i] == '|')
		{
			if (check_consecutive_pipes(str, i + 1))
			{
				fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
				return (0);
			}
		}
		// Handle redirections
		else if (str[i] == '<' || str[i] == '>')
		{
			if (!check_redirection_syntax(str, i))
				return (0);
			while (str[i] == '<' || str[i] == '>')
				i++;
			i--; // Compensate for the upcoming i++
		}
		i++;
	}

	return (check_empty_pipe_segments(str));
}

// Enhanced syntax checking function
int	check_cmd1(char *str)
{
	int		i;
	char	quote;
	char	last_token;
	int		token_expected;

	if (!str || !*str)
		return (1);

	i = 0;
	last_token = 0;
	token_expected = 0;

	// Skip leading whitespace
	i = skip_whitespace(str, i);

	// Check if command starts with a pipe
	if (str[i] == '|')
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
		return (0);
	}

	while (str[i])
	{
		// Skip whitespace
		if (str[i] == ' ' || str[i] == '\t')
		{
			i++;
			continue;
		}

		// Handle quotes
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			
			if (!str[i])
			{
				fprintf(stderr, "minishell: syntax error: unclosed %s quote\n", 
					(quote == '\'') ? "single" : "double");
				return (0);
			}
			
			token_expected = 0;
			last_token = 'W'; // Word
		}
		// Handle pipes
		else if (str[i] == '|')
		{
			// Check if there's a token before the pipe
			if (last_token == 0 || last_token == '|' || token_expected)
			{
				fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
				return (0);
			}
			
			
			// Check for consecutive pipe operators
			int pipe_count = 1;
			int j = i + 1;
			
			// Count consecutive pipes
			while (str[j] == '|')
			{
				pipe_count++;
				j++;
			}
			
			// Handle different cases
			if (pipe_count == 2)
			{
				// || operator - for now treat as syntax error since we don't support it
				fprintf(stderr, "minishell: syntax error near unexpected token `&&'\n");
				return (0);
			}
			else if (pipe_count > 2)
			{
				// More than 2 pipes is an error
				fprintf(stderr, "minishell: syntax error near unexpected token `||'\n");
				return (0);
			}
			// Single pipe (pipe_count == 1) is handled normally
			
			// Check for pipe at the end by looking ahead
			int next = skip_whitespace(str, i + 1);
			if (!str[next])
			{
				fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
				return (0);
			}
			
		last_token = '|';
		token_expected = 1; // Expecting a command after pipe
	}
		// Handle redirections
		else if (str[i] == '<' || str[i] == '>')
		{
			// Use check_redirection_syntax for consistency
			if (!check_redirection_syntax(str, i))
				return (0);
			
			// Skip past the redirection operators
			while (str[i] == '<' || str[i] == '>')
				i++;
			
			last_token = 'R'; // Redirection
			token_expected = 1; // Expecting a filename after redirection
			i--; // Compensate for the upcoming i++
		}
		else
		{
			// Regular character - consider it part of a word
			token_expected = 0;
			last_token = 'W'; // Word
		}
		i++;
	}

	// Check if the command ends with a redirection without a target
	if (token_expected)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
		return (0);
	}

	return (1);
}

// Count number of commands (pipe segments)
int	count_pipes(char *cmd)
{
	int		i;
	int		count;
	char	quote;

	i = 0;
	count = 1;
	while (cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
		{
			quote = cmd[i++];
			while (cmd[i] && cmd[i] != quote)
				i++;
		}
		else if (cmd[i] == '|')
		{
			// Check if this is a || (logical OR) operator
			if (cmd[i + 1] == '|')
			{
				// Skip the second | to avoid double counting
				i++;
			}
			else
			{
				// This is a regular pipe, count it
				count++;
			}
		}
		i++;
	}
	return (count);
}
