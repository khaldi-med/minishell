#ifndef MINISHELL_H
#define MINISHELL_H

#include "../src/utils/libft/libft.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#define MAX_PATH 1024

/* Custom error codes */
#define SUCCESS 0
#define ERROR 1
#define SYNTAX_ERROR 2
#define MS_CMD_NOT_FOUND 127

/* Error message macros */
#define CMD_NOT_FOUND "command not found"
#define NO_SUCH_FILE "No such file or directory"
#define PERMISSION_DENIED "Permission denied"
#define NOT_A_DIRECTORY "Not a directory"
#define TOO_MANY_ARGS "too many arguments"
#define INVALID_IDENTIFIER "not a valid identifier"
#define NUMERIC_REQUIRED "numeric argument required"
#define MS_SYNTAX_ERROR "syntax error near unexpected token"
#define UNCLOSED_QUOTE "unexpected EOF while looking for matching"

/* Token types */
typedef enum e_token_type {
  TOKEN_WORD,
  TOKEN_PIPE,
  TOKEN_REDIR_IN,
  TOKEN_REDIR_OUT,
  TOKEN_REDIR_APPEND,
  TOKEN_HEREDOC,
} t_token_type;

/* just for norminette*/
typedef struct s_quote_state {
  int i;
  int j;
  char quote_char;
} t_quote_state;

/* Token structure */
typedef struct s_token {
  char *value;
  t_token_type type;
  struct s_token *next;
} t_token;

/* Redirection structure */
typedef struct s_redir {
  t_token_type type;
  char *file;
  struct s_redir *next;
} t_redir;

/* Command structure */
typedef struct s_cmd {
  char **args;
  t_redir *redirs;
  struct s_cmd *next;
} t_cmd;

/* Shell structure */
typedef struct s_shell {
  char **env;
  int exit_status;
  t_cmd *cmds;
} t_shell;

/* Executor functions */
void ft_exec_cmds(t_shell *shell, t_cmd *cmds);
void ft_exec_sing_cmd(t_shell *shell, t_cmd *cmd);
void ft_exec_pipe(t_shell *shell, t_cmd *cmds);
void ft_wait_for_processes(t_shell *shell, pid_t last_pid);
void ft_exec_external_cmd(t_cmd *cmd, char **env);
int ft_setup_redirections(t_redir *redirs);
/* int ft_is_valid_number(char *str); */
int ft_exec_built_with_redir(t_shell *shell, t_cmd *cmd);
void ft_wait_for_child(pid_t pid, t_shell *shell);
void ft_handle_no_args(t_shell *shell, t_cmd *cmd);

/* Built-in commands */
int ft_is_builtin(char *cmd);
int ft_exec_builtin(t_shell *shell, char **args);
int ft_builtin_echo(char **args);
int ft_builtin_cd(t_shell *shell, char **args);
int ft_cd_handle_home(char **path, t_shell *shell);
int ft_cd_handle_oldpwd(char **path, t_shell *shell);

int ft_builtin_pwd(char **args);
int ft_builtin_export(t_shell *shell, char **args);
int ft_builtin_unset(t_shell *shell, char **args);
int ft_builtin_env(t_shell *shell);
int ft_builtin_exit(t_shell *shell, char **args);

/* Exit utility functions */
char *ft_strtrim_whitespace(char *str);
int ft_atol(char *str, long *result);
int ft_is_valid_exit_arg(char *str);

/* Environment functions */
char *ft_get_env_value(char **env, char *key);
void ft_set_env_value(t_shell *shell, char *key, char *value);
void ft_unset_env_value(t_shell *shell, char *key);
char **ft_copy_env(char **env);

/* Expansion functions */
char *ft_expand_variables(char *str, t_shell *shell, t_token_type token_type);
char *ft_extract_quoted_var(char *str, int *i);
char *ft_expand_dollar(char *str, int *i, t_shell *shell);
char *ft_handle_dollar_expansion(char *str, int *i, char *result,
                                 t_shell *shell);
char *ft_get_variable_value(char *var, t_shell *shell);
int ft_in_singlea_q(char *s, int pos);

/* Quote handling */
char *ft_handle_quotes(char *str);
char ft_check_unclosed_quotes(char *str);

/* Path resolution */
char *ft_find_executable(char *cmd, char **env);
char **ft_get_path_dirs(char **env);
int ft_is_executable(char *path);

/* Heredoc functions */
int ft_preprocess_heredocs(t_cmd *cmds);
char *ft_create_heredoc_file(char *delimiter);
void ft_cleanup_heredoc_file(char *filename);
char *ft_create_temp_filename(void);
char *ft_read_heredoc_input(char *delimiter);
int ft_process_heredoc_redir(t_redir *redir);

/* Signal functions */
void ft_setup_signals(void);
void ft_signal_handler(int sig);

/* Lexer and parser functions */
t_token *ft_tokenize(char *input);
t_token *ft_creat_token(char *value, t_token_type type);
void ft_add_token(t_token **tokens, t_token *new_token);
t_token *ft_handle_redirection(char *input, int *i);
int ft_skip_space(char *input, int i);
t_cmd *ft_parse_tokens(t_token *tokens, t_shell *shell);
void ft_free_tokens(t_token *tokens);
void ft_free_cmds(t_cmd *cmds);

/* Parse utility functions */
void ft_process_word_token(t_cmd *cmd, t_token *token, int *index,
                           t_shell *shell);
int ft_count_args(t_token *tokens);
void ft_process_redir_token(t_cmd *cmd, t_token **token, t_shell *shell);
char *ft_strjoin_with_newline(char *s1, char *s2);

/* Command creation functions */
t_cmd *ft_creat_cmd(void);
void ft_add_redir(t_cmd *cmd, t_token_type type, char *file);

/* Shell management functions */
void ft_init_shell(t_shell *shell, char **envp);
void ft_cleanup_shell(t_shell *shell);
void ft_shell_loop(t_shell *shell);

// free functions
void ft_free_path_dirs(char **path_dirs);

/* Error handling functions */
void ft_print_error(char *prefix, char *message);
void ft_print_command_error(char *command, char *message);
void ft_print_file_error(char *filename);
void ft_print_syntax_error(char *token);
int ft_handle_export_error(char *identifier);
int ft_is_valid_identifier(char *str);
void ft_print_exit_error(char *arg, char *message);
int ft_validate_input(t_shell *shell, char *input);
#endif
