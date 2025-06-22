#include "../inc/minishell.h"
#include <sys/stat.h>
#include <errno.h>

/*
 * Check if a command is a built-in command
 */
int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0 ||
        ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0 ||
        ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0 ||
        ft_strcmp(cmd, "exit") == 0)
        return (1);
    return (0);
}

/*
 * Execute a built-in command
 */
int execute_builtin(t_bash *bash, t_cmd *cmd, int *exit_status)
{
    if (ft_strcmp(cmd->arguments[0], "echo") == 0)
        *exit_status = builtin_echo(cmd);
    else if (ft_strcmp(cmd->arguments[0], "cd") == 0)
        *exit_status = builtin_cd(bash, cmd);
    else if (ft_strcmp(cmd->arguments[0], "pwd") == 0)
        *exit_status = builtin_pwd();
    else if (ft_strcmp(cmd->arguments[0], "export") == 0)
        *exit_status = builtin_export(bash, cmd);
    else if (ft_strcmp(cmd->arguments[0], "unset") == 0)
        *exit_status = builtin_unset(bash, cmd);
    else if (ft_strcmp(cmd->arguments[0], "env") == 0)
        *exit_status = builtin_env(bash);
    else if (ft_strcmp(cmd->arguments[0], "exit") == 0)
        *exit_status = builtin_exit(bash, cmd);
    return (*exit_status);
}

/*
 * Find the path of a command in the PATH environment variable
 */
char *find_command_path(t_bash *bash, char *cmd)
{
    char *path_env;
    char **paths;
    char *cmd_path;
    int i;

    if (!cmd || !bash)
        return (NULL);
    if (cmd[0] == '/' || cmd[0] == '.')
        return (ft_strdup(cmd)); // Absolute or relative path
    path_env = get_env_value(bash->env_list, "PATH");
    if (!path_env)
        return (NULL);
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);
    i = 0;
    while (paths[i])
    {
        cmd_path = malloc(ft_strlen(paths[i]) + ft_strlen(cmd) + 2);
        if (!cmd_path)
        {
            free_array(paths);
            return (NULL);
        }
        sprintf(cmd_path, "%s/%s", paths[i], cmd);
        if (access(cmd_path, X_OK) == 0)
        {
            free_array(paths);
            return (cmd_path);
        }
        free(cmd_path);
        i++;
    }
    free_array(paths);
    return (NULL);
}

/*
 * Check if a path is a directory
 */
int is_directory(char *path)
{
    struct stat path_stat;
    if (stat(path, &path_stat) != 0)
        return (0);
    return (S_ISDIR(path_stat.st_mode));
}

/*
 * Execute an external command using execve
 */
int execute_external(t_bash *bash, t_cmd *cmd, int *exit_status)
{
    pid_t pid;
    char *cmd_path;
    int status;
    extern int g_exit_status;

    cmd_path = find_command_path(bash, cmd->arguments[0]);
    if (!cmd_path)
    {
        fprintf(stderr, "minishell: %s: command not found\n", cmd->arguments[0]);
        *exit_status = 127;
        g_exit_status = 127;
        return (127);
    }

    // Check if it's a directory
    if (is_directory(cmd_path))
    {
        fprintf(stderr, "minishell: %s: Is a directory\n", cmd->arguments[0]);
        free(cmd_path);
        *exit_status = 126;
        g_exit_status = 126;
        return (126);
    }

    // Check if file has execute permissions
    if (access(cmd_path, X_OK) != 0)
    {
        fprintf(stderr, "minishell: %s: Permission denied\n", cmd->arguments[0]);
        free(cmd_path);
        *exit_status = 126;
        g_exit_status = 126;
        return (126);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(cmd_path);
        *exit_status = 1;
        g_exit_status = 1;
        return (1);
    }
    else if (pid == 0)
    {
        // Child process
        if (execve(cmd_path, cmd->arguments, bash->env_array) == -1)
        {
            perror("execve");
            exit(126);
        }
    }
    else
    {
        // Parent process
        free(cmd_path);
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            *exit_status = WEXITSTATUS(status);
            g_exit_status = WEXITSTATUS(status);
        }
        else if (WIFSIGNALED(status))
        {
            *exit_status = 128 + WTERMSIG(status);
            g_exit_status = 128 + WTERMSIG(status);
        }
    }
    return (*exit_status);
}

/*
 * Main execution function for a single command
 */
int execute_command(t_bash *bash, t_cmd *cmd, int *exit_status)
{
    if (!cmd || !cmd->arguments || !cmd->arguments[0])
        return (ERROR);

    // Apply redirections if needed
    if (cmd->check_red && !apply_redirections(cmd))
        return (ERROR);

    // Execute builtin or external command
    if (is_builtin(cmd->arguments[0]))
        *exit_status = execute_builtin(bash, cmd, exit_status);
    else
        *exit_status = execute_external(bash, cmd, exit_status);

    // Restore original file descriptors
    if (cmd->check_red)
        restore_redirections();

    return (*exit_status);
}

/*
 * Execute all commands in the bash structure
 */
int execute_commands(t_bash *bash)
{
    int exit_status;

    if (!bash || !bash->s_cmd)
        return (ERROR);

    exit_status = 0;
    
    // Single command
    if (bash->num_cmd == 1)
    {
        execute_command(bash, bash->s_cmd[0], &exit_status);
    }
    // Multiple commands with pipes
    else
    {
        execute_pipeline(bash, &exit_status);
    }
    
    return (exit_status);
}

