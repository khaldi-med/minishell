#include "../inc/minishell.h"

/*
 * Store the original stdin and stdout for restoration after redirection
 */
static int g_original_stdin;
static int g_original_stdout;

/*
 * Initialize pipe file descriptors
 */
void init_pipes(int pipes[][2], int num_pipes)
{
    int i;

    i = 0;
    while (i < num_pipes)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        i++;
    }
}

/*
 * Close all pipes except for the specific read and write ends needed for the current command
 */
void close_unused_pipes(int pipes[][2], int num_pipes, int cmd_index)
{
    int i;

    i = 0;
    while (i < num_pipes)
    {
        // Close read end if not the immediate predecessor of the current command
        if (i != cmd_index - 1)
            close(pipes[i][0]);
        
        // Close write end if not the current pipe
        if (i != cmd_index)
            close(pipes[i][1]);
        
        i++;
    }
}

/*
 * Set up input and output redirection for a command in the pipeline
 */
void setup_pipe_redirections(int pipes[][2], int num_pipes, int cmd_index)
{
    // Save original stdin and stdout
    g_original_stdin = dup(STDIN_FILENO);
    g_original_stdout = dup(STDOUT_FILENO);

    // Redirect stdin from previous pipe, if not the first command
    if (cmd_index > 0)
    {
        dup2(pipes[cmd_index - 1][0], STDIN_FILENO);
        close(pipes[cmd_index - 1][0]);
    }

    // Redirect stdout to next pipe, if not the last command
    if (cmd_index < num_pipes)
    {
        dup2(pipes[cmd_index][1], STDOUT_FILENO);
        close(pipes[cmd_index][1]);
    }
}

/*
 * Close all pipes
 */
void close_all_pipes(int pipes[][2], int num_pipes)
{
    int i;

    i = 0;
    while (i < num_pipes)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

/*
 * Restore original stdin and stdout
 */
void restore_pipe_redirections(void)
{
    dup2(g_original_stdin, STDIN_FILENO);
    dup2(g_original_stdout, STDOUT_FILENO);
    close(g_original_stdin);
    close(g_original_stdout);
}

/*
 * Execute a command in a pipeline
 */
void execute_piped_command(t_bash *bash, t_cmd *cmd, int pipes[][2], int cmd_index, int num_pipes)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child process
        setup_pipe_redirections(pipes, num_pipes, cmd_index);
        
        // Apply any additional redirections
        if (cmd->check_red)
            apply_redirections(cmd);
        
        // Close unused pipes
        close_unused_pipes(pipes, num_pipes, cmd_index);
        
        // Execute command
        if (is_builtin(cmd->arguments[0]))
        {
            int exit_status = 0;
            execute_builtin(bash, cmd, &exit_status);
            exit(exit_status);
        }
        else
        {
            char *cmd_path = find_command_path(bash, cmd->arguments[0]);
            if (!cmd_path)
            {
                fprintf(stderr, "minishell: %s: command not found\n", cmd->arguments[0]);
                exit(127);
            }
            
            if (execve(cmd_path, cmd->arguments, bash->env_array) == -1)
            {
                perror("execve");
                free(cmd_path);
                exit(EXIT_FAILURE);
            }
        }
    }
}

/*
 * Execute a pipeline of commands
 */
int execute_pipeline(t_bash *bash, int *exit_status)
{
    int i;
    int num_pipes;
    int pipes[10][2]; // Support up to 10 pipes (11 commands)
    int status;

    if (!bash || !bash->s_cmd || bash->num_cmd <= 0)
        return (ERROR);

    num_pipes = bash->num_cmd - 1;
    
    // Initialize pipes
    init_pipes(pipes, num_pipes);
    
    // Launch each command in the pipeline
    i = 0;
    while (i < bash->num_cmd)
    {
        execute_piped_command(bash, bash->s_cmd[i], pipes, i, num_pipes);
        i++;
    }
    
    // Close all pipes in the parent
    close_all_pipes(pipes, num_pipes);
    
    // Wait for all child processes
    i = 0;
    while (i < bash->num_cmd)
    {
        wait(&status);
        i++;
    }
    
    // Set exit status from the last command
    if (WIFEXITED(status))
        *exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        *exit_status = 128 + WTERMSIG(status);
    
    return (*exit_status);
}

