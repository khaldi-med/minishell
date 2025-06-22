#include "../../inc/minishell.h"
#include <unistd.h>
#include <stdlib.h>

extern int g_exit_status;

/*
 * Helper function to get the length of a variable name
 */
static int get_var_name_length(char *str, int start)
{
    int len = 0;
    int i = start;
    
    // Handle special cases first
    if (str[i] == '?')
        return (1);
    
    // Variable names start with letter or underscore, followed by alphanumeric or underscore
    if (!ft_isalpha(str[i]) && str[i] != '_')
        return (0);
    
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
    {
        len++;
        i++;
    }
    
    return (len);
}

/*
 * Get environment variable value or special variables like $?
 */
static char *get_variable_value(t_bash *bash, char *var_name)
{
    char *value;
    char *exit_str;
    
    if (!var_name)
        return (ft_strdup(""));
    
    // Handle special variable $?
    if (ft_strcmp(var_name, "?") == 0)
    {
        exit_str = malloc(12); // Enough for max int value
        if (!exit_str)
            return (ft_strdup(""));
        sprintf(exit_str, "%d", g_exit_status);
        return (exit_str);
    }
    
    // Handle special variable $PWD
    if (ft_strcmp(var_name, "PWD") == 0)
    {
        char cwd[MAX_PATH_LEN];
        if (getcwd(cwd, MAX_PATH_LEN))
            return (ft_strdup(cwd));
        return (ft_strdup("/home/mohammed"));
    }
    
    // Handle special variable $USER
    if (ft_strcmp(var_name, "USER") == 0)
    {
        char *user = getenv("USER");
        if (user)
            return (ft_strdup(user));
        return (ft_strdup("mohammed"));
    }
    
    // Handle special variable $HOME
    if (ft_strcmp(var_name, "HOME") == 0)
    {
        char *home = getenv("HOME");
        if (home)
            return (ft_strdup(home));
        return (ft_strdup("/home/mohammed"));
    }
    
    // Get regular environment variable
    value = get_env_value(bash->env_list, var_name);
    if (value)
        return (ft_strdup(value));
    
    return (ft_strdup(""));
}



/*
 * Expand environment variables in a string
 */
char *expand_variables(t_bash *bash, char *str)
{
	int i = 0;
	int j = 0;
	int expanded_len;
	char *expanded;
	char *var_name;
	char *var_value;
	int var_name_len;
	int in_single_quotes = 0;
	int in_double_quotes = 0;
	
	if (!str || !bash)
		return (ft_strdup(str ? str : ""));
	
	// First pass: calculate length needed
	expanded_len = ft_strlen(str) * 4 + 2000; // More conservative estimate
	expanded = malloc(expanded_len);
	if (!expanded)
		return (ft_strdup(str));
	
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			expanded[j++] = str[i++]; // Preserve the quote in output
		}
		else if (str[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			expanded[j++] = str[i++]; // Preserve the quote in output
		}
		else if (str[i] == '$' && !in_single_quotes && str[i + 1])
		{
			i++; // Skip the '$'
			var_name_len = get_var_name_length(str, i);
			
			if (var_name_len > 0)
			{
				var_name = ft_substr(str, i, var_name_len);
				if (var_name)
				{
					var_value = get_variable_value(bash, var_name);
					if (var_value)
					{
						int k = 0;
						while (var_value[k] && j < expanded_len - 1)
							expanded[j++] = var_value[k++];
						free(var_value);
					}
					free(var_name);
				}
				i += var_name_len;
			}
			else if (str[i] == '$')
			{
				// Handle isolated $ character  
				expanded[j++] = '$';
				i++;
			}
			else
			{
				// Invalid variable name, keep the '$' as literal
				expanded[j++] = '$';
			}
		}
		else if (str[i] == '$' && str[i + 1] == '\0')
		{
			// Handle $ at end of string
			expanded[j++] = str[i++];
		}
		else if (str[i] == '$' && in_single_quotes)
		{
			// In single quotes, treat $ as literal
			expanded[j++] = str[i++];
		}
		else
		{
			if (j < expanded_len - 1)
				expanded[j++] = str[i++];
			else
				break;
		}
	}
	
	expanded[j] = '\0';
	return (expanded);
}

/*
 * Remove quotes from a final processed string
 */
char *remove_outer_quotes(char *str)
{
    int i = 0;
    int j = 0;
    int len = ft_strlen(str);
    char *result = malloc(len + 1);
    int in_single_quotes = 0;
    int in_double_quotes = 0;
    
    if (!result)
        return ft_strdup(str);
    
    while (str[i])
    {
        if (str[i] == '\'' && !in_double_quotes)
        {
            in_single_quotes = !in_single_quotes;
            i++; // Skip the quote
        }
        else if (str[i] == '"' && !in_single_quotes)
        {
            in_double_quotes = !in_double_quotes;
            i++; // Skip the quote
        }
        else
        {
            result[j++] = str[i++];
        }
    }
    
    result[j] = '\0';
    return result;
}

/*
 * Expand variables in all arguments of a command
 */
void expand_command_variables(t_bash *bash, t_cmd *cmd)
{
    int i;
    char *expanded;
    char *final;
    
    if (!bash || !cmd || !cmd->arguments)
        return;
    
    i = 0;
    while (cmd->arguments[i])
    {
        // First expand variables
        expanded = expand_variables(bash, cmd->arguments[i]);
        if (expanded)
        {
            // Then remove quotes
            final = remove_outer_quotes(expanded);
            free(cmd->arguments[i]);
            free(expanded);
            cmd->arguments[i] = final;
        }
        i++;
    }
}

