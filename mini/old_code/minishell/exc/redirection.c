#include "../inc/minishell.h"
#include <errno.h>

/*
 * Store original file descriptors for restoration
 */
static int g_original_stdin;
static int g_original_stdout;
static int g_original_stderr;

/*
 * Save original file descriptors before redirections
 */
void save_original_fds(void)
{
    g_original_stdin = dup(STDIN_FILENO);
    g_original_stdout = dup(STDOUT_FILENO);
    g_original_stderr = dup(STDERR_FILENO);
}

/*
 * Restore original file descriptors after redirections
 */
void restore_redirections(void)
{
    dup2(g_original_stdin, STDIN_FILENO);
    dup2(g_original_stdout, STDOUT_FILENO);
    dup2(g_original_stderr, STDERR_FILENO);
    close(g_original_stdin);
    close(g_original_stdout);
    close(g_original_stderr);
}

/*
 * Handle input redirection (<)
 */
int handle_input_redirection(char *file)
{
    int fd;
    extern int g_exit_status;

    if (!file || !*file)
    {
        fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
        g_exit_status = 2;
        return (0);
    }
    
    fd = open(file, O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "minishell: %s: %s\n", file, strerror(errno));
        g_exit_status = 1;
        return (0);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    return (1);
}

/*
 * Handle output redirection (>)
 */
int handle_output_redirection(char *file)
{
    int fd;

    if (!file || !*file)
    {
        fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
        return (0);
    }
    
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        fprintf(stderr, "minishell: %s: %s\n", file, strerror(errno));
        return (0);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return (1);
}

/*
 * Handle append redirection (>>)
 */
int handle_append_redirection(char *file)
{
    int fd;

    if (!file || !*file)
    {
        fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
        return (0);
    }
    
    fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        fprintf(stderr, "minishell: %s: %s\n", file, strerror(errno));
        return (0);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return (1);
}

/*
 * Read from standard input until delimiter is encountered (heredoc)
 */
char *read_heredoc(char *delimiter)
{
    char *line;
    char *heredoc;
    char *temp;

    heredoc = ft_strdup("");
    if (!heredoc)
        return (NULL);

    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        temp = heredoc;
        heredoc = ft_strjoin(heredoc, line);
        free(temp);
        temp = heredoc;
        heredoc = ft_strjoin(heredoc, "\n");
        free(temp);
        free(line);
    }
    return (heredoc);
}

/*
 * Handle heredoc redirection (<<)
 */
int handle_heredoc_redirection(char *delimiter)
{
    int fd[2];
    char *heredoc;

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return (0);
    }

    heredoc = read_heredoc(delimiter);
    if (!heredoc)
        return (0);

    write(fd[1], heredoc, ft_strlen(heredoc));
    free(heredoc);
    close(fd[1]);

    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    return (1);
}

/*
 * Apply all redirections for a command
 */
int apply_redirections(t_cmd *cmd)
{
    int i;
    
    if (!cmd || !cmd->s_red)
        return (1);
    
    save_original_fds();
    
    i = 0;
    while (cmd->s_red[i])
    {
        if (cmd->s_red[i]->type == INPUT)
        {
            if (!handle_input_redirection(cmd->s_red[i]->file))
                return (0);
        }
        else if (cmd->s_red[i]->type == OUTPUT)
        {
            if (!handle_output_redirection(cmd->s_red[i]->file))
                return (0);
        }
        else if (cmd->s_red[i]->type == APPEND)
        {
            if (!handle_append_redirection(cmd->s_red[i]->file))
                return (0);
        }
        else if (cmd->s_red[i]->type == HERE_DOC)
        {
            if (!handle_heredoc_redirection(cmd->s_red[i]->file))
                return (0);
        }
        i++;
    }
    return (1);
}

