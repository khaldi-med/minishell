#include "../inc/minishell.h"

// Forward declarations for missing functions
int ft_isalpha(int c);
int ft_isalnum(int c);
char *ft_strchr(const char *str, int c);
int ft_atoi(const char *str);

// Forward declarations for environment functions
int add_env(t_env **env_list, char *key, char *value);
int delete_env(t_env **env_list, char *key);
int update_env(t_bash *bash, char *key, char *value);

// Global environment list (used because t_bash doesn't have env_list in the header)
extern t_env *g_env_list;

/*
 * Display all environment variables
 */
int builtin_env(t_bash *bash)
{
    t_env *current;

    if (!bash || !bash->env_list)
    {
        fprintf(stderr, "env: environment not set\n");
        return (ERROR);
    }
    
    current = bash->env_list;
    while (current)
    {
        printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }
    
    return (SUCCESS);
}

/*
 * Check if a variable name is valid
 */
int is_valid_identifier(char *str)
{
    int i;

    if (!str || !str[0] || !(ft_isalpha(str[0]) || str[0] == '_'))
        return (0);
    
    i = 1;
    while (str[i])
    {
        if (!(ft_isalnum(str[i]) || str[i] == '_'))
            return (0);
        i++;
    }
    
    return (1);
}

/*
 * Print all exported variables in export format
 */
void print_exported_vars(t_bash *bash)
{
    t_env *current;
    
    if (!bash || !bash->env_list)
        return;
        
    current = bash->env_list;
    while (current)
    {
        printf("declare -x %s=\"%s\"\n", current->key, current->value);
        current = current->next;
    }
}

/*
 * Set or display environment variables
 */
int builtin_export(t_bash *bash, t_cmd *cmd)
{
    int i;
    char *key;
    char *value;
    char *equal_sign;
    int has_error = 0;

    if (!cmd->arguments[1])
    {
        print_exported_vars(bash);
        return (SUCCESS);
    }
    
    i = 1;
    while (cmd->arguments[i])
    {
        equal_sign = ft_strchr(cmd->arguments[i], '=');
        if (equal_sign)
        {
            *equal_sign = '\0';
            key = cmd->arguments[i];
            value = equal_sign + 1;
            
            if (!is_valid_identifier(key))
            {
                fprintf(stderr, "export: '%s': not a valid identifier\n", key);
                *equal_sign = '=';
                has_error = 1;
                i++;
                continue;
            }
            
            if (update_env(bash, key, value) == ERROR)
            {
                fprintf(stderr, "export: failed to set variable %s\n", key);
                *equal_sign = '=';
                return (ERROR);
            }
            
            *equal_sign = '=';
        }
        else
        {
            // Handle export without assignment (export VAR)
            if (!is_valid_identifier(cmd->arguments[i]))
            {
                fprintf(stderr, "export: '%s': not a valid identifier\n", cmd->arguments[i]);
                has_error = 1;
            }
            // For export without assignment, just validate but don't set
        }
        i++;
    }
    
    return (has_error ? ERROR : SUCCESS);
}

/*
 * Remove environment variables
 */
int builtin_unset(t_bash *bash, t_cmd *cmd)
{
    int i;

    if (!bash || !cmd->arguments[1])
        return (SUCCESS);
    
    i = 1;
    while (cmd->arguments[i])
    {
        if (!is_valid_identifier(cmd->arguments[i]))
        {
            fprintf(stderr, "unset: '%s': not a valid identifier\n", cmd->arguments[i]);
            i++;
            continue;
        }
        
        if (delete_env(&(bash->env_list), cmd->arguments[i]) == SUCCESS)
        {
            g_env_list = bash->env_list;  // Update global list after successful deletion
        }
        i++;
    }
    
    return (SUCCESS);
}

/*
 * Check if string is a valid number
 */
int is_valid_number(char *str)
{
    int i = 0;
    
    if (!str || !str[0])
        return (0);
    
    // Handle optional sign
    if (str[i] == '+' || str[i] == '-')
        i++;
    
    // Must have at least one digit after optional sign
    if (!str[i])
        return (0);
    
    // Check all remaining characters are digits
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    
    return (1);
}

/*
 * Exit the shell with a status code
 */
int builtin_exit(t_bash *bash, t_cmd *cmd)
{
    (void)bash; // Suppress unused parameter warning
    int exit_code;

    printf("exit\n");
    
    if (!cmd->arguments[1])
        exit(0);
    
    // Check if argument is a valid number
    if (!is_valid_number(cmd->arguments[1]))
    {
        fprintf(stderr, "exit: %s: numeric argument required\n", cmd->arguments[1]);
        exit(2);
    }
    
    if (cmd->arguments[2])
    {
        fprintf(stderr, "exit: too many arguments\n");
        return (ERROR);
    }
    
    exit_code = ft_atoi(cmd->arguments[1]);
    exit(exit_code);
    return (SUCCESS); // This line will never be executed
}

