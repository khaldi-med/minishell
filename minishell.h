/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammed                                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-04                              #+#    #+#             */
/*   Updated: 2025-05-05                             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Standard Libraries */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

/* Define Constants */
# define MAX_PATH_LEN 4096
# define MAX_ARGS 128
# define PROMPT "minishell$ "
# define SUCCESS 0
# define ERROR 1

/* Token Types for Lexer */
typedef enum e_token_type
{
	TOKEN_WORD,           /* Simple word (command, argument, etc.) */
	TOKEN_PIPE,           /* | */
	TOKEN_REDIRECT_IN,    /* < */
	TOKEN_REDIRECT_OUT,   /* > */
	TOKEN_APPEND_OUT,     /* >> */
	TOKEN_HEREDOC,        /* << */
	TOKEN_AND,            /* && */
	TOKEN_OR,             /* || */
	TOKEN_SEMICOLON,      /* ; */
	TOKEN_ENV_VARIABLE,   /* $VAR */
	TOKEN_QUOTE,          /* " or ' */
	TOKEN_SPACE,          /* Whitespace */
	TOKEN_INVALID         /* Invalid token */
} t_token_type;

/* Token Structure */
typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token  *next;
} t_token;

/* Command Structure */
typedef struct s_command
{
    char            **args;     /* Command and its arguments */
    int             in_fd;      /* Input file descriptor */
    int             out_fd;     /* Output file descriptor */
    char            *in_file;   /* Input redirection file */
    char            *out_file;  /* Output redirection file */
    int             append;     /* Flag for append mode (>>) */
    int             heredoc;    /* Flag for heredoc (<<) */
    char            *heredoc_delim; /* Heredoc delimiter */
    struct s_command *next;     /* Next command in pipeline */
} t_command;

/* Environment Variable Structure */
typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
} t_env;

/* Main Shell Structure */
typedef struct s_shell
{
    char            *input;     /* User input string */
    t_token         *tokens;    /* Tokenized input */
    t_command       *commands;  /* Parsed commands */
    t_env           *env_list;  /* Environment variables */
    char            **env_array;/* Environment as string array */
    int             exit_status;/* Last command exit status */
    int             running;    /* Shell running status */
} t_shell;

/* Function Prototypes */

/* Initialization and Cleanup */
t_shell     *init_shell(char **env);
void        free_shell(t_shell *shell);
void        free_tokens(t_token *tokens);
void        free_command_args(char **args);
void        free_command_files(t_command *cmd);
void        free_commands(t_command *cmds);
void        free_env_list(t_env *env);
void        free_env_array(char **env_array);

/* Environment Variable Handling */
t_env       *init_env(char **env);
int         extract_env_pair(char *env_str, char **key, char **value);
int         process_env_var(t_env **env_list, char *env_str);
t_env       *create_env_node(char *key, char *value);
t_env       *add_env(t_env *env_list, char *key, char *value);
char        *get_env_value(t_env *env_list, char *key);
int         update_env(t_env *env_list, char *key, char *value);
char        **allocate_env_array(int count);
char        *create_env_string(t_env *env_node);
char        **env_list_to_array(t_env *env_list);

/* Input Processing */
int         check_exit_command(t_shell *shell);
int         tokenize_and_expand(t_shell *shell);
int         parse_input(t_shell *shell);
void        cleanup_after_execution(t_shell *shell);
int         process_input(t_shell *shell);
void        handle_user_input(t_shell *shell);
void        add_to_history(char *input);

/* Lexer/Tokenizer */
t_token_type get_token_type(char c);
t_token     *create_token(char *value, t_token_type type);
void        add_token(t_token **tokens, t_token *new_token);
void        copy_quoted_content(char *result, char *str, int start, int len);
char        *handle_quotes(char *str, int *i);
void        handle_word(char *input, int *i, t_token **tokens);
void        handle_quoted_text(char *input, int *i, t_token **tokens);
t_token     *tokenize(char *input);

/* Parser */
t_command   *create_command(void);
int         handle_redirections(t_command *cmd, t_token **tokens);
int         handle_arguments(t_command *cmd, t_token *token);
int         expand_variables(t_shell *shell);
void        process_token(t_command **curr, t_token **token);
t_command   *parse_tokens(t_token *tokens);
void        add_command(t_command **commands, t_command *new_cmd);

/* Executor */
int         is_builtin(char *cmd);
int         execute_builtin(t_shell *shell, t_command *cmd);
int         setup_redirections(t_command *cmd);
void        setup_child_fds(int in_fd, int *pipe_fd);
void        execute_command_in_child(t_shell *shell, t_command *cmd);
void        child_process(t_shell *shell, t_command *cmd, int in_fd, int *pipe_fd);
void        parent_process(int in_fd, int *pipe_fd, pid_t pid, int *status);
int         execute_single_command(t_shell *shell, t_command *cmd, int in_fd, int *pipe_fd);
int         init_execution(t_shell *shell, int *in_fd, int *status);
int         setup_pipe_for_next_cmd(t_command *cmd, int pipe_fd[2]);
int         execute_commands(t_shell *shell);

/* Builtins */
int         is_builtin(char *cmd);
int         execute_builtin(t_shell *shell, t_command *cmd);
int         builtin_echo(t_command *cmd);
int         builtin_cd(t_shell *shell, t_command *cmd);
int         builtin_pwd(void);
int         is_valid_identifier(char *str);
int         update_env_array(t_shell *shell);
int         count_env_vars(t_env *env_list);
int         fill_sorted_array(t_env *env_list, char **sorted);
void        print_sorted_env(t_env *env_list, char **sorted, int count);
void        sort_and_print_export(t_env *env_list);
void        process_export_arg(t_shell *shell, char *arg);
int         builtin_export(t_shell *shell, t_command *cmd);
void        remove_env_var(t_shell *shell, char *key);
int         builtin_unset(t_shell *shell, t_command *cmd);
int         builtin_env(t_shell *shell);
int         builtin_exit(t_shell *shell, t_command *cmd);

/* Signal Handling */
void        setup_signals(void);
void        handle_sigint(int sig);
void        handle_sigquit(int sig);

/* Utilities */
char        *find_command_path(char *cmd, t_env *env_list);
char        **split_args(char *str);
int         count_args(char **args);
char        *join_path(char *path, char *cmd);
int         is_directory(char *path);
void        print_error(char *cmd, char *msg);

#endif
