#include "../../inc/minishell.h"

int	check_rd(char c)
{
	if (c == '>' || c == '<' || c == ' ' || c == '\t')
		return (1);
	return (0);
}

int	count_redirection(char *cmd)
{
	int	i;
	int	red;

	i = 0;
	red = 0;
	while (cmd[i])
	{
		if (cmd[i] == '>' || cmd[i] == '<')
		{
			if (cmd[i + 1] == '>' || cmd[i + 1] == '<')
				i += 2;
			else
				i++;
			red++;
		}
		else
			i++;
	}
	return (red);
}

/*
 * Helper function to free allocated redirection structures in case of error
 */
static void	free_allocated_red(t_bash *bash, int i, int n_red)
{
	int	j;

	j = 0;
	while (j < n_red && bash->s_cmd[i]->s_red[j])
	{
		if (bash->s_cmd[i]->s_red[j]->file)
			free(bash->s_cmd[i]->s_red[j]->file);
		free(bash->s_cmd[i]->s_red[j]);
		j++;
	}
	free(bash->s_cmd[i]->s_red);
	bash->s_cmd[i]->s_red = NULL;
}

int	alloc_s_red(t_bash *bash, int n_red, int i)
{
	int	x;

	x = 0;
	bash->s_cmd[i]->s_red = malloc(sizeof(t_red *) * (n_red + 1));
	if (!bash->s_cmd[i]->s_red)
		return (0);
	while (x < n_red)
	{
		bash->s_cmd[i]->s_red[x] = malloc(sizeof(t_red));
		if (!bash->s_cmd[i]->s_red[x])
		{
			free_allocated_red(bash, i, x);
			return (0);
		}
		bash->s_cmd[i]->s_red[x]->type = 0;
		bash->s_cmd[i]->s_red[x]->file = NULL;
		x++;
	}
	bash->s_cmd[i]->s_red[x] = NULL;
	return (1);
}

/*
 * Set redirection type based on operator
 */
static void	set_redirection_type(t_red *red, char c1, char c2)
{
	if (c1 == '>')
	{
		if (c2 == '>')
			red->type = APPEND;
		else
			red->type = OUTPUT;
	}
	else if (c1 == '<')
	{
		if (c2 == '<')
			red->type = HERE_DOC;
		else
			red->type = INPUT;
	}
}

/*
 * Simple quote state update function
 */
static int	update_quote_state(char *str, int pos, int *in_quote, char *quote)
{
	if (!*in_quote && (str[pos] == '\'' || str[pos] == '\"'))
	{
		*in_quote = 1;
		*quote = str[pos];
		return (1);
	}
	else if (*in_quote && str[pos] == *quote)
	{
		*in_quote = 0;
		*quote = 0;
		return (1);
	}
	return (0);
}


/*
 * Identify redirection types in a command string
 */
void	redirection_types(t_bash *bash, int i)
{
	int		j;
	int		x;
	char	c1, c2;

	j = 0;
	x = 0;
	while (bash->s_cmd[i]->command[j] && x < count_redirection(bash->s_cmd[i]->command))
	{
		// Skip characters until we find a redirection operator
		if (bash->s_cmd[i]->command[j] != '>' && bash->s_cmd[i]->command[j] != '<')
		{
			j++;
			continue;
		}

		// Get current and next character
		c1 = bash->s_cmd[i]->command[j];
		c2 = bash->s_cmd[i]->command[j + 1];

		// Set redirection type
		set_redirection_type(bash->s_cmd[i]->s_red[x], c1, c2);

		// Skip past redirection operators
		if (c1 == c2)
			j += 2;
		else
			j++;

		x++;
	}
}

/*
 * Parse redirection filenames
 */
void	redirection_files(t_bash *bash, int i)
{
	int		j;
	int		x;
	int		start;
	int		len;
	char	quote;
	int		in_quote;

	j = 0;
	x = 0;
	while (bash->s_cmd[i]->command[j] && x < count_redirection(bash->s_cmd[i]->command))
	{
		// Skip characters until we find a redirection operator
		if (bash->s_cmd[i]->command[j] != '>' && bash->s_cmd[i]->command[j] != '<')
		{
			j++;
			continue;
		}

		// Skip past redirection operators (handle ">>" or "<<")
		if (bash->s_cmd[i]->command[j + 1] == bash->s_cmd[i]->command[j])
			j += 2;
		else
			j++;

		// Skip whitespace before filename
		while (bash->s_cmd[i]->command[j] && 
			(bash->s_cmd[i]->command[j] == ' ' || bash->s_cmd[i]->command[j] == '\t'))
			j++;

		// Mark start of filename
		start = j;
		len = 0;
		in_quote = 0;
		quote = 0;

		// First pass: calculate filename length, handling quotes properly
		while (bash->s_cmd[i]->command[j])
		{
			// Update quote state and skip quote delimiters
			if (update_quote_state(bash->s_cmd[i]->command, j, &in_quote, &quote))
			{
				j++;
				continue;
			}

			// Break if we hit a delimiter outside of quotes
			if (!in_quote && (check_rd(bash->s_cmd[i]->command[j]) || 
				bash->s_cmd[i]->command[j] == '|'))
				break;

			// Count character for the filename
			len++;
			j++;
		}

		// Allocate memory for filename
		if (len > 0)
		{
			bash->s_cmd[i]->s_red[x]->file = malloc(len + 1);
			if (!bash->s_cmd[i]->s_red[x]->file)
				return;

			// Second pass: copy filename, handling quotes and escapes
			j = start;
			len = 0;
			in_quote = 0;
			quote = 0;

			while (bash->s_cmd[i]->command[j])
			{
				// Update quote state and skip quote delimiters
				if (update_quote_state(bash->s_cmd[i]->command, j, &in_quote, &quote))
				{
					j++;
					continue;
				}

				// Break if we hit a delimiter outside of quotes
				if (!in_quote && (check_rd(bash->s_cmd[i]->command[j]) || 
					bash->s_cmd[i]->command[j] == '|'))
					break;

				// Normal character - just copy it
				bash->s_cmd[i]->s_red[x]->file[len++] = bash->s_cmd[i]->command[j++];
			}

			// Null terminate the filename
			bash->s_cmd[i]->s_red[x]->file[len] = '\0';
		}
		else
		{
			// Handle empty filename case
			bash->s_cmd[i]->s_red[x]->file = ft_strdup("");
			if (!bash->s_cmd[i]->s_red[x]->file)
				return;
		}
		x++;
	}
}

int	red_parse(t_bash *bash)
{
	int	i;
	int	n_red;

	i = 0;
	while (bash->s_cmd[i])
	{
		// Check for redirections directly instead of relying on check_red
		n_red = count_redirection(bash->s_cmd[i]->command);
		if (n_red > 0)
		{
			if (!alloc_s_red(bash, n_red, i))
				return (0);
			redirection_types(bash, i);
			redirection_files(bash, i);
		}
		i++;
	}
	return (1);
}

/*
 * Remove redirection tokens from arguments array
 */
void remove_redirection_from_args(t_cmd *cmd)
{
    char **new_args;
    int i, j, count;
    int skip_next;
    
    if (!cmd || !cmd->arguments)
        return;
    
    // Count non-redirection arguments
    count = 0;
    i = 0;
    skip_next = 0;
    
    while (cmd->arguments[i])
    {
        if (skip_next)
        {
            skip_next = 0;
            i++;
            continue;
        }
        
        // Check if this is a redirection operator
        if (ft_strcmp(cmd->arguments[i], ">") == 0 ||
            ft_strcmp(cmd->arguments[i], "<") == 0 ||
            ft_strcmp(cmd->arguments[i], ">>") == 0 ||
            ft_strcmp(cmd->arguments[i], "<<") == 0)
        {
            skip_next = 1; // Skip the filename too
        }
        else
        {
            // Check if this argument contains redirection mixed with filename
            char *redir_pos = NULL;
            if ((redir_pos = ft_strchr(cmd->arguments[i], '>')) != NULL ||
                (redir_pos = ft_strchr(cmd->arguments[i], '<')) != NULL)
            {
                // This argument contains embedded redirection, should be filtered
                // but we'll handle this case differently by splitting
                if (redir_pos == cmd->arguments[i])
                {
                    // Redirection at beginning, skip this arg and next
                    skip_next = 1;
                    i++;
                    continue;
                }
            }
            count++;
        }
        i++;
    }
    
    // Allocate new arguments array
    new_args = malloc(sizeof(char *) * (count + 1));
    if (!new_args)
        return;
    
    // Copy non-redirection arguments
    i = 0;
    j = 0;
    skip_next = 0;
    
    while (cmd->arguments[i])
    {
        if (skip_next)
        {
            skip_next = 0;
            i++;
            continue;
        }
        
        // Check if this is a redirection operator
        if (ft_strcmp(cmd->arguments[i], ">") == 0 ||
            ft_strcmp(cmd->arguments[i], "<") == 0 ||
            ft_strcmp(cmd->arguments[i], ">>") == 0 ||
            ft_strcmp(cmd->arguments[i], "<<") == 0)
        {
            skip_next = 1; // Skip the filename too
        }
        else
        {
            // Check for embedded redirection
            char *redir_pos = NULL;
            if ((redir_pos = ft_strchr(cmd->arguments[i], '>')) != NULL ||
                (redir_pos = ft_strchr(cmd->arguments[i], '<')) != NULL)
            {
                if (redir_pos == cmd->arguments[i])
                {
                    // Redirection at beginning, skip this arg and next
                    skip_next = 1;
                    i++;
                    continue;
                }
                else
                {
                    // Extract the part before redirection
                    char *clean_arg = malloc(redir_pos - cmd->arguments[i] + 1);
                    if (clean_arg)
                    {
                        strncpy(clean_arg, cmd->arguments[i], redir_pos - cmd->arguments[i]);
                        clean_arg[redir_pos - cmd->arguments[i]] = '\0';
                        new_args[j] = clean_arg;
                        j++;
                    }
                }
            }
            else
            {
                new_args[j] = ft_strdup(cmd->arguments[i]);
                j++;
            }
        }
        i++;
    }
    new_args[j] = NULL;
    
    // Free old arguments and assign new ones
    free_array(cmd->arguments);
    cmd->arguments = new_args;
}
